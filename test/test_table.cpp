#include "../gmock-1.6.0/include/gmock/gmock.h"
#include "../Table.h"
#include <string>
#include <vector>

using namespace std;
using namspace ::testing;

vector<string> get_attribute_names() {
	vector<string> names;
	names.push_back("Ol' Rock");
	names.push_back("56");
	names.push_back("Super Senior");
	names.push_back("4.0");
	names.push_back("2013/01/01");
	names.push_back("-2");
	names.push_back("-5.2");
	names.push_back("13:55:23");
	return names;
}

vector<string> get_attribute_names() {
	string attr_array[] = { "Name", "Age", "Grade", "GPA", "GraduationDate", "BooksOwed", "DiningDollars", "LastLoginTime" };
  return vector<string> attr(attr_array, attr_array + 8);
}

vector<string> get_attribute_types() {
	string type_array[] = { "string", "integer", "string", "float", "date", "integer", "float", "time" };
	return vector<string> type(type_array, type_array + 8);
}

vector<string> get_single_record() {
  string jane_array[] = { "Jane Smith", "19", "Sophomore", "2.8", "2015/05/06", "-2", "-1.28", "01:44:12" };
  return vector<string> jane(jane_array, jane_array + 5);
}

Table shared_table;
void setup_shared_table() {
	shared_table = Table(get_attribute_names(), get_attribute_types());
}

void setup_shared_table_with_data() {
	setup_shared_table();

  string jane_array[] = { "Jane Smith", "19", "Sophomore", "2.8", "2015/05/06", "-2", "-1.28", "01:44:12" };
  vector<string> jane(jane_array, jane_array + 5);

  string jack_array[] = { "Jack Smith", "21", "Senior",    "3.2", "2013/05/06", "4",  "67.45", "14:56:34" };
  vector<string> jack(jack_array, jack_array + 5);

  string jim_array[] =  { "Jim Smith",  "21", "Senior",    "3.1", "2013/05/06", "-5", "20.19", "21:00:00" };
  vector<string> jim(jim_array, jim_array + 5);

  shared_table.insert(jane);
  shared_table.insert(jack);
  shared_table.insert(jim);
}

/* Test Construction of Tables */
TEST(TableConstructionTest, CreatesEmptyTableWithNoRows) {
	Table* table;
	EXPECT_NO_THROW(table = new Table());
	EXPECT_EQ(table->size(), 0);
	delete table;
}

TEST(TableConstructionTest, CreatesPredefinedTableWithNoRows) {
	EXPECT_NO_THROW(setup_shared_table());
	EXPECT_EQ(shared_table.size(), 0);
}

/* Test Adding attributes to Table */
TEST(AddAttributeTest, Success) {
	setup_shared_table();
	EXPECT_EQ(TRUE, shared_table.add("72", "integer"));
}

TEST(AddAttributeTest, FailsOnNonIntegerString) {
	setup_shared_table();
	EXPECT_EQ(FALSE, shared_table.add("lala", "integer"));
}

/* Test Deleting attributes from Table */
TEST(DeleteAttributeTest, Success) {
	setup_shared_table();
	EXPECT_EQ(TRUE, shared_table.deleteAttribute("GraduationDate"));
}

TEST(DeleteAttributeTest, FailsForNonexistentAttribute) {
	setup_shared_table();
	EXPECT_EQ(FALSE, shared_table.deleteAttribute("Woo"));
}

TEST(DeleteAttributeTest, DeletesAttribute) {
	setup_shared_table();
	EXPECT_THAT(shared_table.attributes(), Contains("GraduationDate"));
	shared_table.deleteAttribute("GraduationDate");
	EXPECT_THAT(shared_table.attributes(), Not(Contains("GraduationDate")));
}

/* Inserting Record to Table */
TEST(InsertNewRecord, Success) {
	setup_shared_table();
	vector<string> new_record = get_single_record();
	EXPECT_EQ(TRUE, shared_table.insert(new_record));
}

TEST(InsertNewRecord, FailsForWrongVectorSize) {
	setup_shared_table();
	vector<int> new_record;
	new_record.push_back(4);
	new_record.push_back(3);
	EXPECT_EQ(FALSE, shared_table.insert(new_record));
}

TEST(InsertNewRecord, IncrementsSize) {
	setup_shared_table();
	vector<string> new_record = get_single_record();
	int size = shared_table.size();
	shared_table.insert(new_record);
	EXPECT_EQ(shared_table.size(), size+1);
}

TEST(Attributes, ReturnsAttributesList) {
	setup_shared_table();
	EXPECT_EQ(shared_table.attributes(), get_attribute_names());
}

/* Test retrieving attributes */
// QUESTION:
// Does this return the attribute Names or attribute Types?
// Function descrption is unclear, but I have assumed Names
TEST(RetrieveAttributeVector, ReturnsAttributeNames) {
	setup_shared_table();
	vector<string> names = get_attribute_names();
	EXPECT_THAT(names, Each( shared_table.attributes() ));
}

/* Test size correctness */
TEST(TableSize, ReturnsNumberTableEntries) {
	setup_shared_table();
	vector<string> names = get_attribute_names();
	EXPECT_EQ(names.size(), shared_table.attributes().size());
}

/* Test renaming attributes */
TEST(RenamingAttribute, Success) {
	setup_shared_table();
	EXPECT_EQ(TRUE, shared_table.rename("Grade", "Classification"));
}

TEST(RenamingAttribute, FailsForNonexistentAttribute) {
	setup_shared_table();
	EXPECT_EQ(FALSE, shared_table.rename("anothercolumn", "somethingelse"));
}

/* Test the joining of two tables */
// not sure how to test this yet
// possibly make another "known" version of the final cross joined table
Test(CrossJoin, HasAllAttributes) {
	setup_shared_table();

	vector<string> names, types;
	names.push_back("abc"); types.push_back("Date");
	names.push_back("def"); types.push_back("Integer");
	names.push_back("ghi"); types.push_back("String");
	Table other_table(names, types);

	Table joined = shared_table.cross_join(other_table);
	vector<string> attributes = joined.attributes();
	vector<string> join_names(names);
	vector<string> names2(get_attribute_names());
	join_names.insert(join_names.end(), names2.begin(), names2.end());
	sort(join_names.begin(), join_names.end());
	EXPECT_THAT(attributes, WhenSorted(ContainerEq(join_names)));
}

/* Counting of attributes */
TEST(CountAttribute, Success) {
	setup_shared_table_with_data();
	EXPECT_EQ(3, shared_table.sum("Age"));
}

TEST(CountAttribute, FailsForNonexistentAttribute) {
	setup_shared_table_with_data();
	EXPECT_THROW(shared_table.sum("Woo"));
}

/* Summation of attirbutes */
TEST(SumAttribute, Success) {
	setup_shared_table_with_data();
	EXPECT_EQ(61, shared_table.sum("Age"));
}

TEST(SumAttribute, FailsForNonexistentAttribute) {
	setup_shared_table_with_data();
	EXPECT_THROW(shared_table.sum("Woo"));
}

/* Finding minimum attribute entry */
TEST(MinAttribute, Success) {
	setup_shared_table_with_data();
	EXPECT_EQ(19, shared_table.min("Age"));
}

TEST(MinAttribute, FailsForNonexistentAttribute) {
	setup_shared_table_with_data();
	EXPECT_THROW(shared_table.min("Woo"));
}

/* Finding maximum attribute entry */
TEST(MaxAttribute, Success) {
	setup_shared_table_with_data();
	EXPECT_EQ(21, shared_table.max("Age"));
}

TEST(MaxAttribute, FailsForNonexistentAttribute) {
	setup_shared_table_with_data();
	EXPECT_THROW(shared_table.max("Woo"));
}

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
	names.push_back("13:55:23")
	return names;
}

vector<string> get_attribute_types() {
	vector<string> types;
	types.push_back("Word");
	types.push_back("Integer");
	types.push_back("Word");
	types.push_back("Double");
	types.push_back("Date");
	types.push_back("NegativeInteger");
	types.push_back("NegativeDouble");
	types.push_back("Time");
	return types;
}

Table shared_table;
void setup_shared_table() {
	shared_table = Table(get_attribute_names(), get_attribute_types());
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
	EXPECT_EQ(TRUE, shared_table.add("72", "Integer"));
}

TEST(AddAttributeTest, FailsOnNonIntegerString) {
	setup_shared_table();
	EXPECT_EQ(FALSE, shared_table.add("lala", "Integer"));
}

/* Test Deleting attributes from Table */
TEST(DeleteAttributeTest, Success) {
	setup_shared_table();
	EXPECT_EQ(TRUE, shared_table.deleteAttribute("Date"));
}

TEST(DeleteAttributeTest, FailsForNonexistentAttribute) {
	setup_shared_table();
	EXPECT_EQ(FALSE, shared_table.deleteAttribute("int"));
}

TEST(DeleteAttributeTest, DeletesAttribute) {
	setup_shared_table();
	EXPECT_THAT(shared_table.attributes(), Contains("Date"));
	shared_table.deleteAttribute("Date");
	EXPECT_THAT(shared_table.attributes(), Not(Contains("Date")));
}

/* Inserting Record to Table */
TEST(InsertNewRecord, Success) {
	setup_shared_table();
	vector<string> new_record = get_attribute_names();
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
	vector<string> new_record = get_attribute_names();
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
	EXPECT_EQ(names, shared_table.attributes());
}

/* Test size correctness */
TEST(TableSize, ReturnsNumberTableEntries) {
	setup_shared_table();
	vector<string> names = get_attribute_names();
	EXPECT_EQ(names.size(), shared_table.size());
}

/* Test renaming attributes */
TEST(RenamingAttribute, Success) {
	setup_shared_table();
	EXPECT_EQ(TRUE, shared_table.rename("Super Senior", "Red Shirt Freshman"));
}

TEST(RenamingAttribute, FailsForNonexistentAttribute) {
	setup_shared_table();
	EXPECT_EQ(FALSE, shared_table.rename("anothercolumn", "somethingelse"));
}

/* Test the joining of two tables */
// not sure how to test this yet
// possibly make another "known" version of the final cross joined table

/* Summation of attirbutes */

/* Finding minimum attribute entry */

/* Finding maximum attribute entry */

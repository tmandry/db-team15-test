#include "gmock/gmock.h"
#include "../Table.h"
#include "../Error.h"
#include <string>
#include <vector>

using namespace std;
using namespace ::testing;

class TableTest : public Test
{
protected:
  virtual void SetUp() {
    string attr_array[] = { "Name", "Age", "Grade", "GPA", "GraduationDate", "BooksOwed", "DiningDollars", "LastLoginDate" };
    auto attribute_names = vector<string>(attr_array, attr_array+8);
    Table::TYPE type_array[] = { Table::STRING, Table::INT, Table::STRING, Table::FLOAT, Table::DATE, Table::INT, Table::FLOAT, Table::DATE };
    auto attribute_types = vector<Table::TYPE>(type_array, type_array + 8);

    string jane_array[] = { "Jane Smith", "19", "Sophomore", "2.8", "2015/05/06", "-2", "-1.28", "01:44:12" };
    single_record = vector<string>(jane_array, jane_array + 5);
  }

  virtual void TearDown() {
    // reset shared_table in case it was initialized
    shared_table = Table();
  }

  void setup_shared_table() {
    shared_table = Table(attribute_names, attribute_types);
  }

  int setup_shared_table_with_data() {
    setup_shared_table();

    string jane_array[] = { "Jane Smith", "19", "Sophomore", "2.8", "2015/05/06", "-2", "-1.28", "01:44:12" };
    vector<string> jane(jane_array, jane_array + 5);

    string jack_array[] = { "Jack Smith", "21", "Senior",    "3.2", "2013/05/06", "4",  "67.45", "14:56:34" };
    vector<string> jack(jack_array, jack_array + 5);

    string jim_array[] =  { "Jim Smith",  "21", "Senior",    "3.1", "2013/05/06", "-5", "20.19", "21:00:00" };
    vector<string> jim(jim_array, jim_array + 5);

    string bill_array[] = { "Bill Smith", "NULL","Junior",   "3.1", "2013/05/06", "-5", "20.19", "21:00:00" };
    vector<string> bill(bill_array, bill_array + 5);

    shared_table.insert(jane);
    shared_table.insert(jack);
    shared_table.insert(jim);
    shared_table.insert(bill);
    return 3;
  }

  Table shared_table;
  vector<string> attribute_names;
  vector<Table::TYPE> attribute_types;
  vector<string> single_record;
};

/* Test Construction of Tables */
TEST_F(TableTest, DefaultConstructorCreatesEmptyTableWithNoRows) {
  Table* table;
  EXPECT_NO_THROW(table = new Table());
  EXPECT_EQ(table->size(), 0);
  delete table;
}

TEST_F(TableTest, ConstructorCreatesPredefinedTableWithNoRows) {
  EXPECT_NO_THROW(setup_shared_table());
  EXPECT_EQ(shared_table.size(), 0);
}

/* Test Adding attributes to Table */
TEST_F(TableTest, AddWorks) {
  setup_shared_table();
  int attributes_size = shared_table.attributes().size();
  EXPECT_TRUE(shared_table.add("72", Table::INT));
  EXPECT_EQ(shared_table.attributes().size(), attributes_size+1);
}

TEST_F(TableTest, AddFailsOnNonIntegerString) {
  setup_shared_table();
  EXPECT_FALSE(shared_table.add("lala", Table::INT));
}

/* Test Deleting attributes from Table */
TEST_F(TableTest, DeleteAttributeWorks) {
  setup_shared_table();
  EXPECT_THAT(shared_table.attributes(), Contains(Key("GraduationDate")));
  EXPECT_TRUE(shared_table.deleteAttribute("GraduationDate"));
  EXPECT_THAT(shared_table.attributes(), Not(Contains(Key("GraduationDate"))));
}

TEST_F(TableTest, DeleteAttributeFailsForNonexistentAttribute) {
  setup_shared_table();
  EXPECT_FALSE(shared_table.deleteAttribute("Woo"));
}

/* Inserting Record to Table */
TEST_F(TableTest, InsertWorksAndIncrementsSize) {
  setup_shared_table();
  int size = shared_table.size();
  EXPECT_TRUE(shared_table.insert(single_record));
  EXPECT_EQ(shared_table.size(), size+1);
}

TEST_F(TableTest, InsertFailsForWrongVectorSize) {
  setup_shared_table();
  vector<string> new_record;
  new_record.push_back("4");
  new_record.push_back("3");
  EXPECT_FALSE(shared_table.insert(new_record));
}

/* Test retrieving attributes */
// TODO pending changes
/*
TEST_F(TableTest, AttributesReturnsAttributesList) {
  setup_shared_table();
  EXPECT_THAT(shared_table.attributes(), ContainerEq(attribute_names));
}
*/

/* Test size correctness */
TEST_F(TableTest, SizeReturnsNumberOfTableEntries) {
  int size = setup_shared_table_with_data();
  EXPECT_EQ(shared_table.size(), size);
}

/* Test renaming attributes */
TEST_F(TableTest, RenameWorksOnEmptyTable) {
  setup_shared_table();
  EXPECT_TRUE(shared_table.rename("Grade", "Classification"));
}

TEST_F(TableTest, RenameFailsForNonexistentAttribute) {
  setup_shared_table();
  EXPECT_FALSE(shared_table.rename("anothercolumn", "somethingelse"));
}

/* Test the joining of two tables */
// TODO pending changes
/*
TEST_F(TableTest, CrossJoinHasAllAttributes) {
  setup_shared_table();

  vector<string> names;
  vector<Table::TYPE> types;
  names.push_back("abc"); types.push_back(Table::DATE);
  names.push_back("def"); types.push_back(Table::INT);
  names.push_back("ghi"); types.push_back(Table::STRING);
  Table other_table(names, types);
  Table joined = shared_table.crossJoin(other_table);

  vector<Table::TYPE> attribute_types = joined.attributes();
  vector<Table::TYPE> join_types(types);
  join_types.insert(join_types.end(), attribute_types.begin(), attribute_types.end());
  sort(join_types.begin(), join_types.end());
  EXPECT_THAT(attribute_types, WhenSorted(ContainerEq(join_types)));

//  vector<string> attributes = joined.attributes();
//  vector<string> join_names(names);
//  join_names.insert(join_names.end(), attribute_names.begin(), attribute_names.end());
//  sort(join_names.begin(), join_names.end());
//  EXPECT_THAT(attributes, WhenSorted(ContainerEq(join_names)));
}
*/

/* Counting of attributes */
TEST_F(TableTest, CountAttributeWorks) {
  setup_shared_table_with_data();
  EXPECT_EQ(3, shared_table.count("Age"));
}

TEST_F(TableTest, CountAttributeFailsForNonexistentAttribute) {
  setup_shared_table_with_data();
  EXPECT_THROW(shared_table.count("Woo"), Error);
}

/* Summation of attirbutes */
TEST_F(TableTest, SumWorks) {
  setup_shared_table_with_data();
  EXPECT_EQ(61, shared_table.sum("Age"));
}

TEST_F(TableTest, SumFailsForNonexistentAttribute) {
  setup_shared_table_with_data();
  EXPECT_THROW(shared_table.sum("Woo"), Error);
}

/* Finding minimum attribute entry */
TEST_F(TableTest, MinWorks) {
  setup_shared_table_with_data();
  EXPECT_EQ(19, shared_table.min("Age"));
}

TEST_F(TableTest, MinFailsForNonexistentAttribute) {
  setup_shared_table_with_data();
  EXPECT_THROW(shared_table.min("Woo"), Error);
}

/* Finding maximum attribute entry */
TEST_F(TableTest, MaxWorks) {
  setup_shared_table_with_data();
  EXPECT_EQ(21, shared_table.max("Age"));
}

TEST_F(TableTest, MaxFailsForNonexistentAttribute) {
  setup_shared_table_with_data();
  EXPECT_THROW(shared_table.max("Woo"), Error);
}

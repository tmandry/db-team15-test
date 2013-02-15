#include "gmock/gmock.h"
#include "../Table.h"
#include "../Error.h"
#include <string>
#include <vector>
#include <array>
#include <utility>

using namespace std;
using namespace ::testing;

class TableTest : public Test
{
protected:
  virtual void SetUp() {
    shared_attributes.push_back(make_pair("Name", Table::STRING));
    shared_attributes.push_back(make_pair("Age",  Table::INT));
    shared_attributes.push_back(make_pair("Grade", Table::STRING));
    shared_attributes.push_back(make_pair("GPA", Table::FLOAT));
    shared_attributes.push_back(make_pair("GraduationDate", Table::DATE));
    shared_attributes.push_back(make_pair("BooksOwed", Table::INT));
    shared_attributes.push_back(make_pair("DiningDollars", Table::FLOAT));
    shared_attributes.push_back(make_pair("LastLoginDate", Table::DATE));

    RowArray jane_array = { "Jane Smith", "19", "Sophomore", "2.8", "2015/05/06", "-2", "-1.28", "01:44:12" };
    single_record = vector<string>(jane_array.begin(), jane_array.end());
  }

  virtual void TearDown() {
    // reset shared_table in case it was initialized
    shared_table = Table();
  }

  void setup_shared_table() {
    shared_table = Table(shared_attributes);
  }

  // returns number of rows
  int setup_shared_table_with_data() {
    setup_shared_table();

    RowArray jane_array = { "Jane Smith", "19", "Sophomore", "2.8", "2015/05/06", "-2", "-1.28", "01:44:12" };
    vector<string> jane(jane_array.begin(), jane_array.end());

    RowArray jack_array = { "Jack Smith", "21", "Senior",    "3.2", "2013/05/06", "4",  "67.45", "14:56:34" };
    vector<string> jack(jack_array.begin(), jack_array.end());

    RowArray jim_array =  { "Jim Smith",  "21", "Senior",    "3.1", "2013/05/06", "-5", "20.19", "21:00:00" };
    vector<string> jim(jim_array.begin(), jim_array.end());

    RowArray bill_array = { "Bill Smith", "NULL","Junior",   "3.1", "2013/05/06", "-5", "20.19", "21:00:00" };
    vector<string> bill(bill_array.begin(), bill_array.end());

    shared_table.insert(jane);
    shared_table.insert(jack);
    shared_table.insert(jim);
    shared_table.insert(bill);
    return 4;
  }

  typedef array<string, 8> RowArray;

  Table shared_table;
  vector<pair<string, Table::TYPE>> shared_attributes;
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
  EXPECT_TRUE(shared_table.add("AnotherColumn", Table::INT));
  EXPECT_EQ(shared_table.attributes().size(), attributes_size+1);
  EXPECT_THAT(shared_table.attributes().back(), Pair("AnotherColumn", Table::INT));
}

/* Test Deleting attributes from Table */
TEST_F(TableTest, DeleteAttributeWorks) {
  setup_shared_table();
  EXPECT_THAT(shared_table.attributes(), Contains( Pair("GraduationDate", _) ));
  EXPECT_TRUE(shared_table.deleteAttribute("GraduationDate"));
  EXPECT_THAT(shared_table.attributes(), Not(Contains( Pair("GraduationDate", _) )));
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
TEST_F(TableTest, AttributesReturnsAttributesList) {
  setup_shared_table();
  EXPECT_THAT(shared_table.attributes(), ContainerEq(shared_attributes));
}

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
TEST_F(TableTest, CrossJoinHasAllAttributes) {
  setup_shared_table();

  vector<pair<string, Table::TYPE>> attributes;
  attributes.push_back(make_pair("abc", Table::DATE));
  attributes.push_back(make_pair("def", Table::INT));
  attributes.push_back(make_pair("ghi", Table::STRING));
  Table other_table(attributes);

  Table joined = shared_table.crossJoin(other_table);

  vector<pair<string, Table::TYPE>> join_attributes(shared_attributes);
  join_attributes.insert(join_attributes.end(), attributes.begin(), attributes.end());
  sort(join_attributes.begin(), join_attributes.end());
  EXPECT_THAT(joined.attributes(), ContainerEq(join_attributes));
}

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

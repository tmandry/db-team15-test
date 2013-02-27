#include "gmock/gmock.h"
#include "Table.h"
#include "TableIterator.h"
#include "Error.h"
#include <string>
#include <vector>
#include <array>
#include <utility>

using namespace std;
using namespace ::testing;

/* custom matchers */
MATCHER_P2(RecordEq, record, num_fields, "") {
  for (int i = 0; i < num_fields; ++i) {
    if (const_cast<Record&>(arg).retrieve(i) != record.retrieve(i))
      return false;
  }
  return true;
}

// Does not detect if record has MORE fields than specified
MATCHER_P(RecordElementsAre, container, "") {
  for (unsigned i = 0; i < container.size(); ++i) {
    if (const_cast<Record&>(arg).retrieve(i) != container[i])
      return false;
  }
  return true;
}

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

    RowArray jane_array = { "Jane Smith", "19", "Sophomore", "2.8", "2015/05/06", "-2", "-1.28" };
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

    RowArray jane_array = { "Jane Smith", "19", "Sophomore", "2.8", "2015/05/06", "-2", "-1.28" };
    vector<string> jane(jane_array.begin(), jane_array.end());

    RowArray jack_array = { "Jack Smith", "21", "NULL",    "3.2", "2013/05/06", "4",  "67.45" };
    vector<string> jack(jack_array.begin(), jack_array.end());

    RowArray jim_array =  { "Jim Smith",  "21", "NULL",    "3.1", "2013/05/06", "-5", "20.19" };
    vector<string> jim(jim_array.begin(), jim_array.end());

    RowArray bill_array = { "Bill Smith", "NULL","Junior",   "3.1", "2013/05/06", "-5", "20.19" };
    vector<string> bill(bill_array.begin(), bill_array.end());

    shared_table.insert(jane);
    shared_table.insert(jack);
    shared_table.insert(jim);
    shared_table.insert(bill);
    return 4;
  }

  // Setup a simple table with a single column and 9 rows that are numbered by that column
  void setup_numbered_table() {
    vector<pair<string, Table::TYPE>> attribute;
    attribute.push_back(make_pair("Num", Table::INT));
    shared_table = Table(attribute);

    for (int i = 1; i <= 9; ++i) {
      vector<string> element;
      element.push_back("0");
      element.back()[0] += i;
      shared_table.insert(element);
    }
  }

  typedef array<string, 7> RowArray;
  typedef vector<pair<string, Table::TYPE>> AttributeList;

  Table shared_table;
  AttributeList shared_attributes;
  vector<string> single_record;
};

TEST_F(TableTest, ConstructorCreatesPredefinedTableWithNoRows) {
  EXPECT_NO_THROW(setup_shared_table());
  EXPECT_EQ(shared_table.size(), 0);
}

TEST_F(TableTest, DefaultConstructorCreatesNoAttributes) {
	Table t;
	EXPECT_EQ(t.attributes().size(), 0);
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

TEST_F(TableTest, IteratorPointsToFirstRowAutomatically) {
  setup_numbered_table();
  TableIterator it(shared_table);
  EXPECT_EQ(it.getRecord().retrieve(0), "1");
}

TEST_F(TableTest, IteratorNextWorks) {
  setup_numbered_table();
  TableIterator it(shared_table);
  it.first(); // should be a no-op

  for (int i = 1; i <= 9; ++i) {
    string val = "0";
    val[0] += i;

    EXPECT_EQ(it.getRecord().retrieve(0), val);
    if (i == 9)
      EXPECT_FALSE(it.next());
    else
      EXPECT_TRUE(it.next());
  }
}

TEST_F(TableTest, IteratorFirstWorks) {
  setup_numbered_table();
  TableIterator it(shared_table);

  // advance all the way through table, then go back to start
  while (it.next());
  it.first();

  for (int i = 1; i <= 9; ++i) {
    string val = "0";
    val[0] += i;

    EXPECT_EQ(it.getRecord().retrieve(0), val);
    if (i == 9)
      EXPECT_FALSE(it.next());
    else
      EXPECT_TRUE(it.next());
  }
}

/* Test the joining of two tables */
TEST_F(TableTest, CrossJoinHasAllAttributes) {
  setup_shared_table();

  AttributeList attributes;
  attributes.push_back(make_pair("abc", Table::DATE));
  attributes.push_back(make_pair("def", Table::INT));
  attributes.push_back(make_pair("ghi", Table::STRING));
  Table other_table(attributes);

  Table joined = shared_table.crossJoin(other_table);

  AttributeList join_attributes(shared_attributes);
  join_attributes.insert(join_attributes.end(), attributes.begin(), attributes.end());
  EXPECT_THAT(joined.attributes(), ContainerEq(join_attributes));
}

TEST_F(TableTest, CrossJoinHasAllRows) {
  typedef array<string, 2> Row;
  typedef vector<string> Vec;

  AttributeList a_attrs;
  a_attrs.push_back(make_pair("A1", Table::STRING));
  a_attrs.push_back(make_pair("A2", Table::STRING));
  AttributeList b_attrs;
  b_attrs.push_back(make_pair("B1", Table::STRING));
  b_attrs.push_back(make_pair("B2", Table::STRING));
  Table a(a_attrs);
  Table b(b_attrs);

  Row ra1 = {"a", "b"};
  Row ra2 = {"NULL", "c"};
  Row rb1 = {"NULL", "NULL"};
  Row rb2 = {"d", "e"};
  a.insert(Vec(ra1.begin(), ra1.end()));
  a.insert(Vec(ra2.begin(), ra2.end()));
  b.insert(Vec(rb1.begin(), rb1.end()));
  b.insert(Vec(rb2.begin(), rb2.end()));

  Table joined = a.crossJoin(b);

  EXPECT_EQ(joined.size(), 4);

  array<array<string, 2>, 2> rows = {{ {"a", "b"}, {"NULL", "c"} }};

  array<array<string, 4>, 4> expected_records = {{
    {"a", "b", "NULL", "NULL"},
    {"a", "b", "d", "e"},
    {"NULL", "c", "NULL", "NULL"},
    {"NULL", "c", "d", "e"}
  }};
  TableIterator iterator(joined);
  for (array<string, 4> record : expected_records) {
    EXPECT_THAT(iterator.getRecord(), RecordElementsAre(record));
    iterator.next();
  }
}

TEST_F(TableTest, CrossJoinHasNoRowsWhenOneTableIsEmpty) {
  typedef array<string, 2> Row;
  typedef vector<string> Vec;

  AttributeList a_attrs;
  a_attrs.push_back(make_pair("A1", Table::STRING));
  a_attrs.push_back(make_pair("A2", Table::STRING));
  AttributeList b_attrs;
  b_attrs.push_back(make_pair("B1", Table::STRING));
  b_attrs.push_back(make_pair("B2", Table::STRING));
  Table a(a_attrs);
  Table b(b_attrs);

  Row ra1 = {"a", "b"};
  Row ra2 = {"NULL", "c"};
  a.insert(Vec(ra1.begin(), ra1.end()));
  a.insert(Vec(ra2.begin(), ra2.end()));

  Table joined = a.crossJoin(b);

  EXPECT_EQ(joined.size(), 0);
}

/* count */
TEST_F(TableTest, CountWorksForInt) {
  setup_shared_table_with_data();
  EXPECT_EQ(3, shared_table.count("Age"));
}

TEST_F(TableTest, CountWorksForString) {
  setup_shared_table_with_data();
  EXPECT_EQ(2, shared_table.count("Grade"));
}

TEST_F(TableTest, CountReturnsZeroForEmptyTable) {
  AttributeList a_attrs;
  a_attrs.push_back(make_pair("A1", Table::STRING));
  Table a(a_attrs);

  EXPECT_EQ(a.count("A1"), 0);
}

TEST_F(TableTest, CountFailsForNonexistentAttribute) {
  setup_shared_table_with_data();
  EXPECT_THROW(shared_table.count("Woo"), Error);
}

/* sum */
TEST_F(TableTest, SumWorksForInt) {
  setup_shared_table_with_data();
  EXPECT_THAT(shared_table.sum("Age"), FloatEq(61));
}

TEST_F(TableTest, SumWorksForIntWithNegative) {
  setup_shared_table_with_data();
  EXPECT_THAT(shared_table.sum("BooksOwed"), FloatEq(-8));
}

TEST_F(TableTest, SumWorksForFloat) {
  setup_shared_table_with_data();
  EXPECT_THAT(shared_table.sum("DiningDollars"), FloatEq(106.55F));
}

TEST_F(TableTest, SumReturnsZeroForEmptyTable) {
  AttributeList a_attrs;
  a_attrs.push_back(make_pair("A1", Table::INT));
  Table a(a_attrs);

  EXPECT_EQ(a.sum("A1"), 0);
}

TEST_F(TableTest, SumFailsForNonexistentAttribute) {
  setup_shared_table_with_data();
  EXPECT_THROW(shared_table.sum("Woo"), Error);
}

/* min */
TEST_F(TableTest, MinWorksForInt) {
  setup_shared_table_with_data();
  EXPECT_THAT(shared_table.min("Age"), FloatEq(19));
}

TEST_F(TableTest, MinWorksForFloat) {
  setup_shared_table_with_data();
  EXPECT_THAT(shared_table.min("DiningDollars"), FloatEq(-1.28F));
}

TEST_F(TableTest, MinFailsForNonexistentAttribute) {
  setup_shared_table_with_data();
  EXPECT_THROW(shared_table.min("Woo"), Error);
}

/* max */
TEST_F(TableTest, MaxWorksForInt) {
  setup_shared_table_with_data();
  EXPECT_THAT(shared_table.max("Age"), FloatEq(21));
}

TEST_F(TableTest, MaxWorksForFloat) {
  setup_shared_table_with_data();
  EXPECT_THAT(shared_table.max("DiningDollars"), FloatEq(67.45F));
}

TEST_F(TableTest, MaxFailsForNonexistentAttribute) {
  setup_shared_table_with_data();
  EXPECT_THROW(shared_table.max("Woo"), Error);
}

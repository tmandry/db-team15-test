#include "../gmock-1.6.0/include/gmock/gmock.h"
#include "../Database.h"
#include "../Table.h"

#include <vector>
#include <algorithm>
using namespace std;

vector<string> get_attributes() {
  // NOTE: If we can use C++11, we can use vector literals instead
  string attr_array[] = { "Name", "Age", "Grade", "GPA", "GraduationDate" };
  vector<string> attr(attr_array, attr_array + 5);

  return attr;
}

// Don't know what these types are supposed to be, this is my best guess
vector<string> get_attribute_types() {
  string type_array[] = { "string", "int", "string", "float", "date" };
  vector<string> types(type_array, type_array + 5);

  return types;
}

void get_basic_database() {
  Table students(get_attributes(), get_attribute_types());

  string jane_array[] = { "Jane Smith", "19", "Sophomore", "2.8", "2015/05/06" };
  vector<string> jane(jane_array, jane_array + 5);

  string jack_array[] = { "Jack Smith", "21", "Senior",    "3.2", "2013/05/06" };
  vector<string> jack(jack_array, jack_array + 5);

  string jim_array[] =  { "Jim Smith",  "21", "Senior",    "3.1", "2013/05/06" };
  vector<string> jim(jim_array, jim_array + 5);

  students.insert(jane);
  students.insert(jack);
  students.insert(jim);

  Database database;
  database.addTable(students, "students");

  return database;
}

/* Construction of Database */
TEST(DatabaseConstructionTest, CreatesEmptyDatabase) {
  EXPECT_NO_THROW(Database());
}

/* Managment of Tables */
TEST(DatabaseTableTest, AddsTables) {
  Table students(get_attributes(), get_attribute_types());
  Database database;

  EXPECT_NO_THROW(database.addTable(students, "students"));
  EXPECT_EQ(1, database.getTableNames().size());
}

TEST(DatabaseTableTest, DropsTables) {
  Table students(get_attributes(), get_attribute_types());
  Database database;
  database.addTable(students, "students");
  EXPECT_EQ(1, database.getTableNames().size());

  EXPECT_NO_THROW(database.dropTable("students"));
  EXPECT_EQ(0, database.getTableNames.size());
}

TEST(DatabaseTableTest, GetsEmptyTableNames) {
  Database database;
  EXPECT_EQ(database.getTableNames().size(), 0);
}

TEST(DatabaseTableTest, GetsTableNames) {
  Table students(get_attributes(), get_attribute_types());
  Database database;
  database.addTable(students, "students");

  EXPECT_EQ(database.getTableNames()[0], "students")
}

TEST(DatabaseTableTest, GetsEmptyTables) {
  Database database;
  EXPECT_EQ(database.getTables().size(), 0);
}

TEST(DatabaseTableTest, GetsTables) {
  Table students(get_attributes(), get_attribute_types());
  Database database;
  database.addTable(students, "students");

  EXPECT_EQ(database.getTables()[0], students);
}

/* Query Database */

// SELECT *
TEST(DatabaseQueryTest, HandlesBasicQuery) {
  Database database = get_basic_database();
  Table query;

  EXPECT_NO_THROW(query = database.query("*", "students", ""));
  EXPECT_TRUE( equal( query.attributes().begin(),
                      query.attributes().end(),
                      get_attributes.begin() ));
  EXPECT_EQ(query.size(), 3);
}

// SELECT 2
TEST(DatabaseQueryTest, HandlesSelectQuery) {
  Database database = get_basic_database();
  Table query;

  string attr_array[] = { "Name", "Age" };
  vector<string> attr(attr_array, attr_array + 2);

  EXPECT_NO_THROW(query = database.query("Name, Age", "students", ""));
  EXPECT_TRUE( equal( query.attributes().begin(),
                      query.attributes().end(),
                      attr.begin() ));
  EXPECT_EQ(query.size(), 3);
}

// SELECT 0
TEST(DatabaseQueryTest, HandlesSelectNoneQuery) {
  Database database = get_basic_database();
  Table query;
  vector<string> attr;

  EXPECT_NO_THROW(query = database.query("", "students", ""));
  EXPECT_TRUE( equal( query.attributes().begin(),
                      query.attributes().end(),
                      attr.begin() ));
  EXPECT_EQ(query.size(), 3);
}

// SELECT *, WHERE =
TEST(DatabaseQueryTest, HandlesWhereEqQuery) {
  Database database = get_basic_database();
  Table query;

  EXPECT_NO_THROW(query = database.query("*", "students", "Age = 19"));
  EXPECT_TRUE( equal( query.attributes().begin(),
                      query.attributes().end(),
                      get_attributes.begin() ));
  EXPECT_EQ(query.size(), 1);
  // Need a way to test that the correct record was returned, but there is no
  // way to get individual records from a table
}

// SELECT *, WHERE !=
TEST(DatabaseQueryTest, HandlesWhereNEqQuery) {
  Database database = get_basic_database();
  Table query;

  EXPECT_NO_THROW(query = database.query("*", "students", "Age != 19"));
  EXPECT_TRUE( equal( query.attributes().begin(),
                      query.attributes().end(),
                      get_attributes.begin() ));
  EXPECT_EQ(query.size(), 2);
  // Need a way to test that the correct record was returned, but there is no
  // way to get individual records from a table. Iterator is currently private.
}

// SELECT *, WHERE >
TEST(DatabaseQueryTest, HandlesWhereGtQuery) {
  Database database = get_basic_database();
  Table query;

  EXPECT_NO_THROW(query = database.query("*", "students", "Age > 19"));
  EXPECT_TRUE( equal( query.attributes().begin(),
                      query.attributes().end(),
                      get_attributes.begin() ));
  EXPECT_EQ(query.size(), 2);
  // Need a way to test that the correct record was returned, but there is no
  // way to get individual records from a table. Iterator is currently private.
}

// SELECT *, WHERE <
TEST(DatabaseQueryTest, HandlesWhereLtQuery) {
  Database database = get_basic_database();
  Table query;

  EXPECT_NO_THROW(query = database.query("*", "students", "Age < 21"));
  EXPECT_TRUE( equal( query.attributes().begin(),
                      query.attributes().end(),
                      get_attributes.begin() ));
  EXPECT_EQ(query.size(), 1);
  // Need a way to test that the correct record was returned, but there is no
  // way to get individual records from a table. Iterator is currently private.
}

// SELECT *, WHERE >=
TEST(DatabaseQueryTest, HandlesWhereGteQuery) {
  Database database = get_basic_database();
  Table query;

  EXPECT_NO_THROW(query = database.query("*", "students", "Age >= 19"));
  EXPECT_TRUE( equal( query.attributes().begin(),
                      query.attributes().end(),
                      get_attributes.begin() ));
  EXPECT_EQ(query.size(), 3);
  // Need a way to test that the correct record was returned, but there is no
  // way to get individual records from a table. Iterator is currently private.
}

// SELECT *, WHERE <=
TEST(DatabaseQueryTest, HandlesWhereLteQuery) {
  Database database = get_basic_database();
  Table query;

  EXPECT_NO_THROW(query = database.query("*", "students", "Age <= 19"));
  EXPECT_TRUE( equal( query.attributes().begin(),
                      query.attributes().end(),
                      get_attributes.begin() ));
  EXPECT_EQ(query.size(), 1);
  // Need a way to test that the correct record was returned, but there is no
  // way to get individual records from a table. Iterator is currently private.
}

/* Multiple conditions on queries, there could be so many combinations here to test */

// SELECT *, WHERE = AND =
TEST(DatabaseQueryTest, HandlesWhereEqAndEqQuery) {
  Database database = get_basic_database();
  Table query;

  EXPECT_NO_THROW(query = database.query("*", "students", "Age = 19 AND Name = 'Jane Smith'"));
  EXPECT_TRUE( equal( query.attributes().begin(),
                      query.attributes().end(),
                      get_attributes.begin() ));
  EXPECT_EQ(query.size(), 1);
  // Need a way to test that the correct record was returned, but there is no
  // way to get individual records from a table. Iterator is currently private.
}

// SELECT *, WHERE > OR =
TEST(DatabaseQueryTest, HandlesWhereEqOrEqQuery) {
  Database database = get_basic_database();
  Table query;

  EXPECT_NO_THROW(query = database.query("*", "students", "Age > 19 OR Name = 'Jane Smith'"));
  EXPECT_TRUE( equal( query.attributes().begin(),
                      query.attributes().end(),
                      get_attributes.begin() ));
  EXPECT_EQ(query.size(), 3);
  // Need a way to test that the correct record was returned, but there is no
  // way to get individual records from a table. Iterator is currently private.
}

// SELECT *, WHERE > NOT >=
TEST(DatabaseQueryTest, HandlesWhereGteNotGteQuery) {
  Database database = get_basic_database();
  Table query;

  EXPECT_NO_THROW(query = database.query("*", "students", "Age > 19 NOT GPA >= 4.0"));
  EXPECT_TRUE( equal( query.attributes().begin(),
                      query.attributes().end(),
                      get_attributes.begin() ));
  EXPECT_EQ(query.size(), 2);
  // Need a way to test that the correct record was returned, but there is no
  // way to get individual records from a table. Iterator is currently private.
}

// SELECT *, WHERE - 1 level of parenthesis
TEST(DatabaseQueryTest, HandlesOneNestedWhereQuery) {
  Database database = get_basic_database();
  Table query;

  EXPECT_NO_THROW(query = database.query("*", "students", "Age = 19 OR ( Age >= 21 AND GPA = 3.2 )"));
  EXPECT_TRUE( equal( query.attributes().begin(),
                      query.attributes().end(),
                      get_attributes.begin() ));
  EXPECT_EQ(query.size(), 2);
  // Need a way to test that the correct record was returned, but there is no
  // way to get individual records from a table. Iterator is currently private.
}

// SELECT *, WHERE - 3 levels of parenthesis
TEST(DatabaseQueryTest, HandlesTwoNestedWhereQuery) {
  Database database = get_basic_database();
  Table query;

  EXPECT_NO_THROW(query = database.query("*", "students", "Age = 19 AND ( Name = 'Jack Smith' OR ( GPA > 3.0 AND Grade = 'Senior' ) )"));
  EXPECT_TRUE( equal( query.attributes().begin(),
                      query.attributes().end(),
                      get_attributes.begin() ));
  EXPECT_EQ(query.size(), 0);
  // Need a way to test that the correct record was returned, but there is no
  // way to get individual records from a table. Iterator is currently private.
}
// SELECT *, WHERE - 3 levels of parenthesis
TEST(DatabaseQueryTest, HandlesThreeNestedWhereQuery) {
  Database database = get_basic_database();
  Table query;

  EXPECT_NO_THROW(query = database.query("*", "students", "Age = 19 OR ( Name = 'Jack Smith' AND ( GPA > 3.0 AND ( Grade = 'Senior' OR  GraduationDate = '2013/05/06') )"));
  EXPECT_TRUE( equal( query.attributes().begin(),
                      query.attributes().end(),
                      get_attributes.begin() ));
  EXPECT_EQ(query.size(), 1);
  // Need a way to test that the correct record was returned, but there is no
  // way to get individual records from a table. Iterator is currently private.
}

/* Select some attributes, with conditions */

// SELECT 2, WHERE =
TEST(DatabaseQueryTest, HandlesSelectWhereEqQuery) {
  Database database = get_basic_database();
  Table query;

  string attr_array[] = { "Name", "Age" };
  vector<string> attr(attr_array, attr_array + 2);

  EXPECT_NO_THROW(query = database.query("Name, Age", "students", "Age = 19"));
  EXPECT_TRUE( equal( query.attributes().begin(),
                      query.attributes().end(),
                      attr.begin() ));
  EXPECT_EQ(query.size(), 1);
  // Need a way to test that the correct record was returned, but there is no
  // way to get individual records from a table. Iterator is currently private.
}

/* Deleting of Data */

// SELECT *
TEST(DatabaseDeleteQueryTest, HandlesBasicQuery) {
  Database database = get_basic_database();

  EXPECT_EQ(database.getTables()[0].size(), 3);
  EXPECT_TRUE(database.deleteQuery("*", "students", ""));
  EXPECT_EQ(database.getTables()[0].size(), 0);
}

// SELECT *, WHERE =
TEST(DatabaseDeleteQueryTest, HandlesWhereEqQuery) {
  Database database = get_basic_database();

  EXPECT_EQ(database.getTables()[0].size(), 3);
  EXPECT_TRUE(database.deleteQuery("*", "students", "Age = 19"));
  EXPECT_EQ(database.getTables()[0].size(), 2);
}

// SELECT *, WHERE !=
TEST(DatabaseDeleteQueryTest, HandlesWhereNEqQuery) {
  Database database = get_basic_database();

  EXPECT_EQ(database.getTables()[0].size(), 3);
  EXPECT_TRUE(database.deleteQuery("*", "students", "Age != 19"));
  EXPECT_EQ(database.getTables()[0].size(), 1);
}

// SELECT *, WHERE <
TEST(DatabaseDeleteQueryTest, HandlesWhereLtQuery) {
  Database database = get_basic_database();

  EXPECT_EQ(database.getTables()[0].size(), 3);
  EXPECT_TRUE(database.deleteQuery("*", "students", "Age < 19"));
  EXPECT_EQ(database.getTables()[0].size(), 3);
}

// SELECT *, WHERE >
TEST(DatabaseDeleteQueryTest, HandlesWhereGtQuery) {
  Database database = get_basic_database();

  EXPECT_EQ(database.getTables()[0].size(), 3);
  EXPECT_TRUE(database.deleteQuery("*", "students", "Age > 19"));
  EXPECT_EQ(database.getTables()[0].size(), 1);
}

// SELECT *, WHERE <=
TEST(DatabaseDeleteQueryTest, HandlesWhereLteQuery) {
  Database database = get_basic_database();

  EXPECT_EQ(database.getTables()[0].size(), 3);
  EXPECT_TRUE(database.deleteQuery("*", "students", "Age <= 19"));
  EXPECT_EQ(database.getTables()[0].size(), 2);
}

// SELECT *, WHERE >=
TEST(DatabaseDeleteQueryTest, HandlesWhereGteQuery) {
  Database database = get_basic_database();

  EXPECT_EQ(database.getTables()[0].size(), 3);
  EXPECT_TRUE(database.deleteQuery("*", "students", "Age >= 19"));
  EXPECT_EQ(database.getTables()[0].size(), 0);
}

// SELECT *, WHERE - 1 level of parenthesis
TEST(DatabaseDeleteQueryTest, HandlesOneNestedWhereQuery) {
  Database database = get_basic_database();

  EXPECT_EQ(database.getTables()[0].size(), 3);
  EXPECT_TRUE(database.deleteQuery("*", "students", "Age = 19 OR ( Age >= 21 AND GPA = 3.2 )"));
  EXPECT_EQ(database.getTables()[0].size(), 1);
}

// SELECT *, WHERE =
TEST(DatabaseDeleteQueryTest, HandlesThreeNestedWhereEqQuery) {
  Database database = get_basic_database();

  EXPECT_EQ(database.getTables()[0].size(), 3);
  EXPECT_TRUE(database.deleteQuery("*", "students", "Age = 19 AND ( Name = 'Jack Smith' OR ( GPA > 3.0 AND Grade = 'Senior' ) )"));
  EXPECT_EQ(database.getTables()[0].size(), 3);
}

// SELECT *, WHERE =
TEST(DatabaseDeleteQueryTest, HandlesThreeNestedWhereEqQuery) {
  Database database = get_basic_database();

  EXPECT_EQ(database.getTables()[0].size(), 3);
  EXPECT_TRUE(database.deleteQuery("*", "students", "Age = 19 OR ( Name = 'Jack Smith' AND ( GPA > 3.0 AND ( Grade = 'Senior' OR  GraduationDate = '2013/05/06') )"));
  EXPECT_EQ(database.getTables()[0].size(), 1);
}
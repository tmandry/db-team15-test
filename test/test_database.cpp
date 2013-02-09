#include "../gmock-1.6.0/include/gmock/gmock.h"
#include "../Database.h"
#include "../Table.h"

#include <vector>
#include <algorithm>
using namespace std;

vector<string> get_attributes() {
  vector<string> attr;
  attr.push_back("Name");
  attr.push_back("Age");
  attr.push_back("Grade");
  attr.push_back("GPA");
  attr.push_back("GraduationDate");

  return attr;
}

// I don't know what these types are supposed to be, this is my best guess
vector<string> get_attribute_types() {
  vector<string> types;
  types.push_back("string");
  types.push_back("int");
  types.push_back("string");
  types.push_back("float");
  types.push_back("date");

  return types;
}

void get_basic_database() {
  Table students(get_attributes(), get_attribute_types());
  vector<string> jane;
  jane.push_back("Jane Smith");
  jane.push_back("19");
  jane.push_back("Sophomore");
  jane.push_back("2.8");
  jane.push_back("05/06/2015");

  vector<string> jack;
  jack.push_back("Jack Smith");
  jack.push_back("21");
  jack.push_back("Senior");
  jack.push_back("3.2");
  jack.push_back("05/06/2013");

  vector<string> jim;
  jack.push_back("Jim Smith");
  jack.push_back("21");
  jack.push_back("Senior");
  jack.push_back("3.1");
  jack.push_back("05/06/2013");

  students.insert(jane);
  students.insert(jack);

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

TEST(DatabaseTableTest, GetsTableNames) {
  Table students(get_attributes(), get_attribute_types());
  Database database;
  database.addTable(students, "students");

  EXPECT_EQ(database.getTableNames()[0], "students")
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

  vector<string> attr;
  attr.push_back("Name");
  attr.push_back("Age");

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
TEST(DatabaseQueryTest, HandlesWhereEQuery) {
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
TEST(DatabaseQueryTest, HandlesWhereNEQuery) {
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
TEST(DatabaseQueryTest, HandlesWhereGTQuery) {
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
TEST(DatabaseQueryTest, HandlesWhereLTQuery) {
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
TEST(DatabaseQueryTest, HandlesWhereGTEQuery) {
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
TEST(DatabaseQueryTest, HandlesWhereLTEQuery) {
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
TEST(DatabaseQueryTest, HandlesWhereEAndEQuery) {
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
TEST(DatabaseQueryTest, HandlesWhereEOrEQuery) {
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
TEST(DatabaseQueryTest, HandlesWhereGTNotGTEQuery) {
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

  EXPECT_NO_THROW(query = database.query("*", "students", "Age = 19 OR ( Name = 'Jack Smith' AND ( GPA > 3.0 AND ( Grade = 'Senior' OR  GraduationDate = '05/06/2013') )"));
  EXPECT_TRUE( equal( query.attributes().begin(),
                      query.attributes().end(),
                      get_attributes.begin() ));
  EXPECT_EQ(query.size(), 1);
  // Need a way to test that the correct record was returned, but there is no
  // way to get individual records from a table. Iterator is currently private.
}

/* Select some attributes, with conditions */

// SELECT 2, WHERE =
TEST(DatabaseQueryTest, HandlesSelectWhereEQuery) {
  Database database = get_basic_database();
  Table query;

  vector<string> attr;
  attr.push_back("Name");
  attr.push_back("Age");

  EXPECT_NO_THROW(query = database.query("Name, Age", "students", "Age = 19"));
  EXPECT_TRUE( equal( query.attributes().begin(),
                      query.attributes().end(),
                      attr.begin() ));
  EXPECT_EQ(query.size(), 1);
  // Need a way to test that the correct record was returned, but there is no
  // way to get individual records from a table. Iterator is currently private.
}
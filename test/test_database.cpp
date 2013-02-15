#include "gmock/gmock.h"
#include "../Database.h"
#include "../Table.h"

#include <vector>
#include <string>
using namespace std;
using namespace ::testing;

vector<string> get_attribute_names() {
  // NOTE: If we can use C++11, we can use vector literals instead
  string attr_array[] = { "Name", "Age", "Grade", "GPA", "GraduationDate" };
  vector<string> attr(attr_array, attr_array + 5);

  return attr;
}

// Don't know what these types are supposed to be, this is my best guess
vector<Table::TYPE> get_attribute_types() {
  Table::TYPE type_array[] = { Table::STRING, Table::INT, Table::STRING, Table::FLOAT, Table::DATE };
  vector<Table::TYPE> types(type_array, type_array + 5);

  return types;
}

vector<pair<string, Table::TYPE> > get_attribute_pairs() {
  vector<string> names = get_attribute_names();
  vector<Table::TYPE> types = get_attribute_types();

  vector<pair<string, Table::TYPE> > pairs;
  for(int i = 0; i < names.size(); i++) {
    pairs.push_back( make_pair(names[i], types[i]));
  }

  return pairs;
}

Database get_basic_database() {
  Table students(get_attribute_pairs());

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
  Table students(get_attribute_names(), get_attribute_types());
  Database database;

  EXPECT_NO_THROW(database.addTable(students, "students"));
  EXPECT_EQ(1, database.getTableNames().size());
}

TEST(DatabaseTableTest, DropsTables) {
  Table students(get_attribute_names(), get_attribute_types());
  Database database;
  database.addTable(students, "students");
  EXPECT_EQ(1, database.getTableNames().size());

  EXPECT_NO_THROW(database.dropTable("students"));
  EXPECT_EQ(0, database.getTableNames().size());
}

TEST(DatabaseTableTest, GetsEmptyTableNames) {
  Database database;
  EXPECT_EQ(database.getTableNames().size(), 0);
}

TEST(DatabaseTableTest, GetsTableNames) {
  Table students(get_attribute_names(), get_attribute_types());
  Database database;
  database.addTable(students, "students");

  EXPECT_EQ(database.getTableNames()[0], "students");
}

TEST(DatabaseTableTest, GetsEmptyTables) {
  Database database;
  EXPECT_EQ(database.getTables().size(), 0);
}

TEST(DatabaseTableTest, GetsTables) {
  Table students(get_attribute_names(), get_attribute_types());
  Database database;
  database.addTable(students, "students");

  EXPECT_EQ(database.getTables().size(), 1);
}

/* Query Database */

// SELECT *
TEST(QueryTest, HandlesBasicQuery) {
  Database database = get_basic_database();
  Table query;

  EXPECT_NO_THROW(query = database.query("*", "students", ""));
  EXPECT_THAT(query.attributes(), ContainerEq( get_attribute_pairs() ));
  EXPECT_EQ(query.size(), 3);
}

// SELECT 2
TEST(QueryTest, HandlesSelectQuery) {
  Database database = get_basic_database();
  Table query;

  string attr_array[] = { "Name", "Age" };
  vector<string> attr(attr_array, attr_array + 2);

  EXPECT_NO_THROW(query = database.query("Name, Age", "students", ""));
  EXPECT_THAT(query.attributes(), ContainerEq( get_attribute_pairs() ));
  EXPECT_EQ(query.size(), 3);
}

// SELECT 0
TEST(QueryTest, HandlesSelectNoneQuery) {
  Database database = get_basic_database();
  Table query;
  vector<string> attr;

  EXPECT_NO_THROW(query = database.query("", "students", ""));
  EXPECT_THAT(query.attributes(), ContainerEq( get_attribute_pairs() ));
  EXPECT_EQ(query.size(), 3);
}

// SELECT *, WHERE =
TEST(QueryTest, HandlesWhereEqQuery) {
  Database database = get_basic_database();
  Table query;

  EXPECT_NO_THROW(query = database.query("*", "students", "Age = 19"));
  EXPECT_THAT(query.attributes(), ContainerEq( get_attribute_pairs() ));
  EXPECT_EQ(query.size(), 1);

  TableIterator it(query);
  Record r = it.getRecord();
  EXPECT_EQ(r.retrieve(0), "Jane Smith");
}

// SELECT *, WHERE !=
TEST(QueryTest, HandlesWhereNEqQuery) {
  Database database = get_basic_database();
  Table query;

  EXPECT_NO_THROW(query = database.query("*", "students", "Age != 19"));
  EXPECT_THAT(query.attributes(), ContainerEq( get_attribute_pairs() ));
  EXPECT_EQ(query.size(), 2);

  TableIterator it(query);
  Record r = it.getRecord();
  EXPECT_EQ(r.retrieve(0), "Jack Smith");

  it.next();
  r = it.getRecord();
  EXPECT_EQ(r.retrieve(0), "Jim Smith");
}

// SELECT *, WHERE >
TEST(QueryTest, HandlesWhereGtQuery) {
  Database database = get_basic_database();
  Table query;

  EXPECT_NO_THROW(query = database.query("*", "students", "Age > 19"));
  EXPECT_THAT(query.attributes(), ContainerEq( get_attribute_pairs() ));
  EXPECT_EQ(query.size(), 2);

  TableIterator it(query);
  Record r = it.getRecord();
  EXPECT_EQ(r.retrieve(0), "Jack Smith");

  it.next();
  r = it.getRecord();
  EXPECT_EQ(r.retrieve(0), "Jim Smith");
}

// SELECT *, WHERE <
TEST(QueryTest, HandlesWhereLtQuery) {
  Database database = get_basic_database();
  Table query;

  EXPECT_NO_THROW(query = database.query("*", "students", "Age < 21"));
  EXPECT_THAT(query.attributes(), ContainerEq( get_attribute_pairs() ));
  EXPECT_EQ(query.size(), 1);

  TableIterator it(query);
  Record r = it.getRecord();
  EXPECT_EQ(r.retrieve(0), "Jane Smith");
}

// SELECT *, WHERE >=
TEST(QueryTest, HandlesWhereGteQuery) {
  Database database = get_basic_database();
  Table query;

  EXPECT_NO_THROW(query = database.query("*", "students", "Age >= 19"));
  EXPECT_THAT(query.attributes(), ContainerEq( get_attribute_pairs() ));
  EXPECT_EQ(query.size(), 3);

  TableIterator it(query);
  Record r = it.getRecord();
  EXPECT_EQ(r.retrieve(0), "Jane Smith");

  it.next();
  r = it.getRecord();
  EXPECT_EQ(r.retrieve(0), "Jack Smith");

  it.next();
  r = it.getRecord();
  EXPECT_EQ(r.retrieve(0), "Jim Smith");
}

// SELECT *, WHERE <=
TEST(QueryTest, HandlesWhereLteQuery) {
  Database database = get_basic_database();
  Table query;

  EXPECT_NO_THROW(query = database.query("*", "students", "Age <= 19"));
  EXPECT_THAT(query.attributes(), ContainerEq( get_attribute_pairs() ));
  EXPECT_EQ(query.size(), 1);

  TableIterator it(query);
  Record r = it.getRecord();
  EXPECT_EQ(r.retrieve(0), "Jane Smith");
}

/* Multiple conditions on queries, there could be so many combinations here to test */

// SELECT *, WHERE = AND =
TEST(QueryTest, HandlesWhereEqAndEqQuery) {
  Database database = get_basic_database();
  Table query;

  EXPECT_NO_THROW(query = database.query("*", "students", "Age = 19 AND Name = 'Jane Smith'"));
  EXPECT_THAT(query.attributes(), ContainerEq( get_attribute_pairs() ));
  EXPECT_EQ(query.size(), 1);

  TableIterator it(query);
  Record r = it.getRecord();
  EXPECT_EQ(r.retrieve(0), "Jane Smith");
}

// SELECT *, WHERE > OR =
TEST(QueryTest, HandlesWhereEqOrEqQuery) {
  Database database = get_basic_database();
  Table query;

  EXPECT_NO_THROW(query = database.query("*", "students", "Age > 19 OR Name = 'Jane Smith'"));
  EXPECT_THAT(query.attributes(), ContainerEq( get_attribute_pairs() ));
  EXPECT_EQ(query.size(), 3);

  TableIterator it(query);
  Record r = it.getRecord();
  EXPECT_EQ(r.retrieve(0), "Jane Smith");

  it.next();
  r = it.getRecord();
  EXPECT_EQ(r.retrieve(0), "Jack Smith");

  it.next();
  r = it.getRecord();
  EXPECT_EQ(r.retrieve(0), "Jim Smith");
}

// SELECT *, WHERE > NOT >=
TEST(QueryTest, HandlesWhereGteNotGteQuery) {
  Database database = get_basic_database();
  Table query;

  EXPECT_NO_THROW(query = database.query("*", "students", "Age > 19 NOT GPA >= 4.0"));
  EXPECT_THAT(query.attributes(), ContainerEq( get_attribute_pairs() ));
  EXPECT_EQ(query.size(), 2);

  TableIterator it(query);
  Record r = it.getRecord();
  EXPECT_EQ(r.retrieve(0), "Jack Smith");

  it.next();
  r = it.getRecord();
  EXPECT_EQ(r.retrieve(0), "Jim Smith");
}

// SELECT *, WHERE - 1 level of parenthesis
TEST(QueryTest, HandlesOneNestedWhereQuery) {
  Database database = get_basic_database();
  Table query;

  EXPECT_NO_THROW(query = database.query("*", "students", "Age = 19 OR ( Age >= 21 AND GPA = 3.2 )"));
  EXPECT_THAT(query.attributes(), ContainerEq( get_attribute_pairs() ));
  EXPECT_EQ(query.size(), 2);

  TableIterator it(query);
  Record r = it.getRecord();
  EXPECT_EQ(r.retrieve(0), "Jane Smith");

  it.next();
  r = it.getRecord();
  EXPECT_EQ(r.retrieve(0), "Jack Smith");
}

// SELECT *, WHERE - 3 levels of parenthesis
TEST(QueryTest, HandlesTwoNestedWhereQuery) {
  Database database = get_basic_database();
  Table query;

  EXPECT_NO_THROW(query = database.query("*", "students", "Age = 19 AND ( Name = 'Jack Smith' OR ( GPA > 3.0 AND Grade = 'Senior' ) )"));
  EXPECT_THAT(query.attributes(), ContainerEq( get_attribute_pairs() ));
  EXPECT_EQ(query.size(), 0);
}
// SELECT *, WHERE - 3 levels of parenthesis
TEST(QueryTest, HandlesThreeNestedWhereQuery) {
  Database database = get_basic_database();
  Table query;

  EXPECT_NO_THROW(query = database.query("*", "students", "Age = 19 OR ( Name = 'Jack Smith' AND ( GPA > 3.0 AND ( Grade = 'Senior' OR  GraduationDate = '2013/05/06') )"));
  EXPECT_THAT(query.attributes(), ContainerEq( get_attribute_pairs() ));
  EXPECT_EQ(query.size(), 1);

  TableIterator it(query);
  Record r = it.getRecord();
  EXPECT_EQ(r.retrieve(0), "Jane Smith");
}

/* Select some attributes, with conditions */

// SELECT 2, WHERE =
TEST(QueryTest, HandlesSelectWhereEqQuery) {
  Database database = get_basic_database();
  Table query;

  string attr_array[] = { "Name", "Age" };
  vector<string> attr(attr_array, attr_array + 2);

  EXPECT_NO_THROW(query = database.query("Name, Age", "students", "Age = 19"));
  EXPECT_THAT(query.attributes(), ContainerEq( get_attribute_pairs() ));
  EXPECT_EQ(query.size(), 1);

  TableIterator it(query);
  Record r = it.getRecord();
  EXPECT_EQ(r.retrieve(0), "Jane Smith");
}

/* Deleting of Data */

// SELECT *
TEST(DeleteQueryTest, HandlesBasicQuery) {
  Database database = get_basic_database();

  EXPECT_EQ(database.getTables()[0].size(), 3);
  EXPECT_TRUE(database.deleteQuery("*", "students", ""));
  EXPECT_EQ(database.getTables()[0].size(), 0);
}

// SELECT *, WHERE =
TEST(DeleteQueryTest, HandlesWhereEqQuery) {
  Database database = get_basic_database();

  EXPECT_EQ(database.getTables()[0].size(), 3);
  EXPECT_TRUE(database.deleteQuery("*", "students", "Age = 19"));
  EXPECT_EQ(database.getTables()[0].size(), 2);
}

// SELECT *, WHERE !=
TEST(DeleteQueryTest, HandlesWhereNEqQuery) {
  Database database = get_basic_database();

  EXPECT_EQ(database.getTables()[0].size(), 3);
  EXPECT_TRUE(database.deleteQuery("*", "students", "Age != 19"));
  EXPECT_EQ(database.getTables()[0].size(), 1);
}

// SELECT *, WHERE <
TEST(DeleteQueryTest, HandlesWhereLtQuery) {
  Database database = get_basic_database();

  EXPECT_EQ(database.getTables()[0].size(), 3);
  EXPECT_TRUE(database.deleteQuery("*", "students", "Age < 19"));
  EXPECT_EQ(database.getTables()[0].size(), 3);
}

// SELECT *, WHERE >
TEST(DeleteQueryTest, HandlesWhereGtQuery) {
  Database database = get_basic_database();

  EXPECT_EQ(database.getTables()[0].size(), 3);
  EXPECT_TRUE(database.deleteQuery("*", "students", "Age > 19"));
  EXPECT_EQ(database.getTables()[0].size(), 1);
}

// SELECT *, WHERE <=
TEST(DeleteQueryTest, HandlesWhereLteQuery) {
  Database database = get_basic_database();

  EXPECT_EQ(database.getTables()[0].size(), 3);
  EXPECT_TRUE(database.deleteQuery("*", "students", "Age <= 19"));
  EXPECT_EQ(database.getTables()[0].size(), 2);
}

// SELECT *, WHERE >=
TEST(DeleteQueryTest, HandlesWhereGteQuery) {
  Database database = get_basic_database();

  EXPECT_EQ(database.getTables()[0].size(), 3);
  EXPECT_TRUE(database.deleteQuery("*", "students", "Age >= 19"));
  EXPECT_EQ(database.getTables()[0].size(), 0);
}

// SELECT *, WHERE - 1 level of parenthesis
TEST(DeleteQueryTest, HandlesOneNestedWhereQuery) {
  Database database = get_basic_database();

  EXPECT_EQ(database.getTables()[0].size(), 3);
  EXPECT_TRUE(database.deleteQuery("*", "students", "Age = 19 OR ( Age >= 21 AND GPA = 3.2 )"));
  EXPECT_EQ(database.getTables()[0].size(), 1);
}

// SELECT *, WHERE =
TEST(DeleteQueryTest, HandlesTwoNestedWhereEqQuery) {
  Database database = get_basic_database();

  EXPECT_EQ(database.getTables()[0].size(), 3);
  EXPECT_TRUE(database.deleteQuery("*", "students", "Age = 19 AND ( Name = 'Jack Smith' OR ( GPA > 3.0 AND Grade = 'Senior' ) )"));
  EXPECT_EQ(database.getTables()[0].size(), 3);
}

// SELECT *, WHERE =
TEST(DeleteQueryTest, HandlesThreeNestedWhereEqQuery) {
  Database database = get_basic_database();

  EXPECT_EQ(database.getTables()[0].size(), 3);
  EXPECT_TRUE(database.deleteQuery("*", "students", "Age = 19 OR ( Name = 'Jack Smith' AND ( GPA > 3.0 AND ( Grade = 'Senior' OR  GraduationDate = '2013/05/06') )"));
  EXPECT_EQ(database.getTables()[0].size(), 1);
}

/******** Integer comparison tests ********/

// SELECT *, WHERE =
TEST(IntegerComparisonTest, HandlesWhereEqQuery) {
  Database database = get_basic_database();
  Table query;

  EXPECT_NO_THROW(query = database.query("*", "students", "Age = 21"));
  EXPECT_THAT(query.attributes(), ContainerEq( get_attribute_pairs() ));
  EXPECT_EQ(query.size(), 2);
}

// SELECT *, WHERE !=
TEST(IntegerComparisonTest, HandlesWhereNEqQuery) {
  Database database = get_basic_database();
  Table query;

  EXPECT_NO_THROW(query = database.query("*", "students", "Age != 21"));
  EXPECT_THAT(query.attributes(), ContainerEq( get_attribute_pairs() ));
  EXPECT_EQ(query.size(), 1);
}

// SELECT *, WHERE >
TEST(IntegerComparisonTest, HandlesWhereGtQuery) {
  Database database = get_basic_database();
  Table query;

  EXPECT_NO_THROW(query = database.query("*", "students", "Age > 21"));
  EXPECT_THAT(query.attributes(), ContainerEq( get_attribute_pairs() ));
  EXPECT_EQ(query.size(), 0);
}

// SELECT *, WHERE <
TEST(IntegerComparisonTest, HandlesWhereLtQuery) {
  Database database = get_basic_database();
  Table query;

  EXPECT_NO_THROW(query = database.query("*", "students", "Age < 21"));
  EXPECT_THAT(query.attributes(), ContainerEq( get_attribute_pairs() ));
  EXPECT_EQ(query.size(), 1);
}

// SELECT *, WHERE >=
TEST(IntegerComparisonTest, HandlesWhereGteQuery) {
  Database database = get_basic_database();
  Table query;

  EXPECT_NO_THROW(query = database.query("*", "students", "Age >= 21"));
  EXPECT_THAT(query.attributes(), ContainerEq( get_attribute_pairs() ));
  EXPECT_EQ(query.size(), 2);
}

// SELECT *, WHERE <=
TEST(IntegerComparisonTest, HandlesWhereLteQuery) {
  Database database = get_basic_database();
  Table query;

  EXPECT_NO_THROW(query = database.query("*", "students", "Age <= 21"));
  EXPECT_THAT(query.attributes(), ContainerEq( get_attribute_pairs() ));
  EXPECT_EQ(query.size(), 3);
}

/******** Float comparison tests ********/

// SELECT *, WHERE =
TEST(FloatComparisonTest, HandlesWhereEqQuery) {
  Database database = get_basic_database();
  Table query;

  EXPECT_NO_THROW(query = database.query("*", "students", "GPA = 3.1"));
  EXPECT_THAT(query.attributes(), ContainerEq( get_attribute_pairs() ));
  EXPECT_EQ(query.size(), 1);
}

// SELECT *, WHERE !=
TEST(FloatComparisonTest, HandlesWhereNEqQuery) {
  Database database = get_basic_database();
  Table query;

  EXPECT_NO_THROW(query = database.query("*", "students", "GPA != 3.1"));
  EXPECT_THAT(query.attributes(), ContainerEq( get_attribute_pairs() ));
  EXPECT_EQ(query.size(), 2);
}

// SELECT *, WHERE >
TEST(FloatComparisonTest, HandlesWhereGtQuery) {
  Database database = get_basic_database();
  Table query;

  EXPECT_NO_THROW(query = database.query("*", "students", "GPA > 3.1"));
  EXPECT_THAT(query.attributes(), ContainerEq( get_attribute_pairs() ));
  EXPECT_EQ(query.size(), 1);
}

// SELECT *, WHERE <
TEST(FloatComparisonTest, HandlesWhereLtQuery) {
  Database database = get_basic_database();
  Table query;

  EXPECT_NO_THROW(query = database.query("*", "students", "GPA < 3.1"));
  EXPECT_THAT(query.attributes(), ContainerEq( get_attribute_pairs() ));
  EXPECT_EQ(query.size(), 1);
}

// SELECT *, WHERE >=
TEST(FloatComparisonTest, HandlesWhereGteQuery) {
  Database database = get_basic_database();
  Table query;

  EXPECT_NO_THROW(query = database.query("*", "students", "GPA >= 3.1"));
  EXPECT_THAT(query.attributes(), ContainerEq( get_attribute_pairs() ));
  EXPECT_EQ(query.size(), 2);
}

// SELECT *, WHERE <=
TEST(FloatComparisonTest, HandlesWhereLteQuery) {
  Database database = get_basic_database();
  Table query;

  EXPECT_NO_THROW(query = database.query("*", "students", "GPA <= 3.1"));
  EXPECT_THAT(query.attributes(), ContainerEq( get_attribute_pairs() ));
  EXPECT_EQ(query.size(), 2);
}

/******** Negative integer comparison tests ********/

// SELECT *, WHERE =
TEST(NegativeIntegerComparisonTest, HandlesWhereEqQuery) {
  Database database = get_basic_database();
  Table query;

  EXPECT_NO_THROW(query = database.query("*", "students", "Age = -21"));
  EXPECT_THAT(query.attributes(), ContainerEq( get_attribute_pairs() ));
  EXPECT_EQ(query.size(), 0);
}

// SELECT *, WHERE !=
TEST(NegativeIntegerComparisonTest, HandlesWhereNEqQuery) {
  Database database = get_basic_database();
  Table query;

  EXPECT_NO_THROW(query = database.query("*", "students", "Age != -21"));
  EXPECT_THAT(query.attributes(), ContainerEq( get_attribute_pairs() ));
  EXPECT_EQ(query.size(), 3);
}

// SELECT *, WHERE >
TEST(NegativeIntegerComparisonTest, HandlesWhereGtQuery) {
  Database database = get_basic_database();
  Table query;

  EXPECT_NO_THROW(query = database.query("*", "students", "Age > -21"));
  EXPECT_THAT(query.attributes(), ContainerEq( get_attribute_pairs() ));
  EXPECT_EQ(query.size(), 3);
}

// SELECT *, WHERE <
TEST(NegativeIntegerComparisonTest, HandlesWhereLtQuery) {
  Database database = get_basic_database();
  Table query;

  EXPECT_NO_THROW(query = database.query("*", "students", "Age < -21"));
  EXPECT_THAT(query.attributes(), ContainerEq( get_attribute_pairs() ));
  EXPECT_EQ(query.size(), 0);
}

// SELECT *, WHERE >=
TEST(NegativeIntegerComparisonTest, HandlesWhereGteQuery) {
  Database database = get_basic_database();
  Table query;

  EXPECT_NO_THROW(query = database.query("*", "students", "Age >= -21"));
  EXPECT_THAT(query.attributes(), ContainerEq( get_attribute_pairs() ));
  EXPECT_EQ(query.size(), 3);
}

// SELECT *, WHERE <=
TEST(NegativeIntegerComparisonTest, HandlesWhereLteQuery) {
  Database database = get_basic_database();
  Table query;

  EXPECT_NO_THROW(query = database.query("*", "students", "Age <= -21"));
  EXPECT_THAT(query.attributes(), ContainerEq( get_attribute_pairs() ));
  EXPECT_EQ(query.size(), 0);
}

/******** Negative float comparison tests ********/

// SELECT *, WHERE =
TEST(NegativeFloatComparisonTest, HandlesWhereEqQuery) {
  Database database = get_basic_database();
  Table query;

  EXPECT_NO_THROW(query = database.query("*", "students", "GPA = -3.1"));
  EXPECT_THAT(query.attributes(), ContainerEq( get_attribute_pairs() ));
  EXPECT_EQ(query.size(), 0);
}

// SELECT *, WHERE !=
TEST(NegativeFloatComparisonTest, HandlesWhereNEqQuery) {
  Database database = get_basic_database();
  Table query;

  EXPECT_NO_THROW(query = database.query("*", "students", "GPA != -3.1"));
  EXPECT_THAT(query.attributes(), ContainerEq( get_attribute_pairs() ));
  EXPECT_EQ(query.size(), 3);
}

// SELECT *, WHERE >
TEST(NegativeFloatComparisonTest, HandlesWhereGtQuery) {
  Database database = get_basic_database();
  Table query;

  EXPECT_NO_THROW(query = database.query("*", "students", "GPA > -3.1"));
  EXPECT_THAT(query.attributes(), ContainerEq( get_attribute_pairs() ));
  EXPECT_EQ(query.size(), 3);
}

// SELECT *, WHERE <
TEST(NegativeFloatComparisonTest, HandlesWhereLtQuery) {
  Database database = get_basic_database();
  Table query;

  EXPECT_NO_THROW(query = database.query("*", "students", "GPA < -3.1"));
  EXPECT_THAT(query.attributes(), ContainerEq( get_attribute_pairs() ));
  EXPECT_EQ(query.size(), 0);
}

// SELECT *, WHERE >=
TEST(NegativeFloatComparisonTest, HandlesWhereGteQuery) {
  Database database = get_basic_database();
  Table query;

  EXPECT_NO_THROW(query = database.query("*", "students", "GPA >= -3.1"));
  EXPECT_THAT(query.attributes(), ContainerEq( get_attribute_pairs() ));
  EXPECT_EQ(query.size(), 3);
}

// SELECT *, WHERE <=
TEST(NegativeFloatComparisonTest, HandlesWhereLteQuery) {
  Database database = get_basic_database();
  Table query;

  EXPECT_NO_THROW(query = database.query("*", "students", "GPA <= -3.1"));
  EXPECT_THAT(query.attributes(), ContainerEq( get_attribute_pairs() ));
  EXPECT_EQ(query.size(), 0);
}

/******** Date comparison tests ********/

// SELECT *, WHERE =
TEST(DateComparisonTest, HandlesWhereEqQuery) {
  Database database = get_basic_database();
  Table query;

  EXPECT_NO_THROW(query = database.query("*", "students", "GraduationDate = 2015/05/06"));
  EXPECT_THAT(query.attributes(), ContainerEq( get_attribute_pairs() ));
  EXPECT_EQ(query.size(), 1);
}

// SELECT *, WHERE !=
TEST(DateComparisonTest, HandlesWhereNEqQuery) {
  Database database = get_basic_database();
  Table query;

  EXPECT_NO_THROW(query = database.query("*", "students", "GraduationDate != 2015/05/06"));
  EXPECT_THAT(query.attributes(), ContainerEq( get_attribute_pairs() ));
  EXPECT_EQ(query.size(), 2);
}

// SELECT *, WHERE >
TEST(DateComparisonTest, HandlesWhereGtQuery) {
  Database database = get_basic_database();
  Table query;

  EXPECT_NO_THROW(query = database.query("*", "students", "GraduationDate > 2015/05/06"));
  EXPECT_THAT(query.attributes(), ContainerEq( get_attribute_pairs() ));
  EXPECT_EQ(query.size(), 0);
}

// SELECT *, WHERE <
TEST(DateComparisonTest, HandlesWhereLtQuery) {
  Database database = get_basic_database();
  Table query;

  EXPECT_NO_THROW(query = database.query("*", "students", "GraduationDate < 2015/05/06"));
  EXPECT_THAT(query.attributes(), ContainerEq( get_attribute_pairs() ));
  EXPECT_EQ(query.size(), 2);
}

// SELECT *, WHERE >=
TEST(DateComparisonTest, HandlesWhereGteQuery) {
  Database database = get_basic_database();
  Table query;

  EXPECT_NO_THROW(query = database.query("*", "students", "GraduationDate >= 2015/05/06"));
  EXPECT_THAT(query.attributes(), ContainerEq( get_attribute_pairs() ));
  EXPECT_EQ(query.size(), 1);
}

// SELECT *, WHERE <=
TEST(DateComparisonTest, HandlesWhereLteQuery) {
  Database database = get_basic_database();
  Table query;

  EXPECT_NO_THROW(query = database.query("*", "students", "GraduationDate <= 3.0"));
  EXPECT_THAT(query.attributes(), ContainerEq( get_attribute_pairs() ));
  EXPECT_EQ(query.size(), 3);
}

/******** String comparison tests ********/

// SELECT *, WHERE =
TEST(StringComparisonTest, HandlesWhereEqQuery) {
  Database database = get_basic_database();
  Table query;

  EXPECT_NO_THROW(query = database.query("*", "students", "Name = 'Jane Smith'"));
  EXPECT_THAT(query.attributes(), ContainerEq( get_attribute_pairs() ));
  EXPECT_EQ(query.size(), 1);
}

// SELECT *, WHERE !=
TEST(StringComparisonTest, HandlesWhereNEqQuery) {
  Database database = get_basic_database();
  Table query;

  EXPECT_NO_THROW(query = database.query("*", "students", "Name != 'Jane Smith'"));
  EXPECT_THAT(query.attributes(), ContainerEq( get_attribute_pairs() ));
  EXPECT_EQ(query.size(), 2);
}

// SELECT *, WHERE >
TEST(StringComparisonTest, HandlesWhereGtQuery) {
  Database database = get_basic_database();
  Table query;

  EXPECT_NO_THROW(query = database.query("*", "students", "Name > 'Jane Smith'"));
  EXPECT_THAT(query.attributes(), ContainerEq( get_attribute_pairs() ));
  EXPECT_EQ(query.size(), 1);
}

// SELECT *, WHERE <
TEST(StringComparisonTest, HandlesWhereLtQuery) {
  Database database = get_basic_database();
  Table query;

  EXPECT_NO_THROW(query = database.query("*", "students", "Name < 'Jane Smith'"));
  EXPECT_THAT(query.attributes(), ContainerEq( get_attribute_pairs() ));
  EXPECT_EQ(query.size(), 1);
}

// SELECT *, WHERE >=
TEST(StringComparisonTest, HandlesWhereGteQuery) {
  Database database = get_basic_database();
  Table query;

  EXPECT_NO_THROW(query = database.query("*", "students", "Name >= 'Jane Smith'"));
  EXPECT_THAT(query.attributes(), ContainerEq( get_attribute_pairs() ));
  EXPECT_EQ(query.size(), 2);
}

// SELECT *, WHERE <=
TEST(StringComparisonTest, HandlesWhereLteQuery) {
  Database database = get_basic_database();
  Table query;

  EXPECT_NO_THROW(query = database.query("*", "students", "Name <= 'Jane Smith'"));
  EXPECT_THAT(query.attributes(), ContainerEq( get_attribute_pairs() ));
  EXPECT_EQ(query.size(), 2);
}

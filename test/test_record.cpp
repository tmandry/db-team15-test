#include "../gmock-1.6.0/gmock.h"
#include "../Record.h"

// This is a where a factory or fixture would be helpful?
Record shared_record;
void setup_shared_record() {
  shared_record = Record(vector<string>(["Ol' Rock", "56", "Super Senior", "4.0", "01/01/2013"]));
}

/* Test Construction of Records */
TEST(RecordConstructionTest, CreatesEmptyRecord) {
  EXPECT_NO_THROW(Record());
}

TEST(RecordConstructionTest, CreatesPredefinedRecord) {
  vector<string> attributes(["Ol' Rock", "56", "Super Senior"]);
  EXPECT_NO_THROW(Record(attributes));
}

/* Test Retrieving of Attributes */
TEST(RecordRetrievalTest, RetrievesStringAttribute) {
  setup_shared_record();

  EXPECT_EQ("Ol' Rock",     shared_record.retreiveRecord(0));
  EXPECT_EQ("Super Senior", shared_record.retreiveRecord(2));
}

// QUESTION: Not sure about int and float, they expect a string to be given to
//           the function, though ideally, the record should convert it for you.
//           Same with retrieval, otherwise, what is the point of the DB having
//           types if you have to convert them? Same problem with modifying.
TEST(RecordRetrievalTest, RetrievesIntegerAttribute) {
  setup_shared_record();

  EXPECT_EQ("56", shared_record.retreiveRecord(1));
}

TEST(RecordRetrievalTest, RetrievesFloatingAttribute) {
  setup_shared_record();

  EXPECT_EQ("4.0", shared_record.retreiveRecord(3));
}

// TODO: test for Date retrieval and Time retrieval

/* Test Modifying of Attributes */
TEST(RecordModifyTest, ModifiesStringAttribute) {
  setup_shared_record();

  shared_record.modifyRecord(2, "Supertastic Senior");
  shared_record.modifyRecord(0, "Ol' Rockie");

  EXPECT_EQ("Supertastic Senior", shared_record.retreiveRecord(2));
  EXPECT_EQ("Ol' Rockie",         shared_record.retreiveRecord(0));
}

// Same typing issues as noted above
TEST(RecordModifyTest, ModifiesIntegerAttribute) {
  setup_shared_record();

  shared_record.modifyRecord(1, "62");

  EXPECT_EQ("62", shared_record.retreiveRecord(1));
}

TEST(RecordModifyTest, ModifiesFloatingAttribute) {
  setup_shared_record();

  shared_record.modifyRecord(3, "4.5");

  EXPECT_EQ("4.5", shared_record.retreiveRecord(3));
}
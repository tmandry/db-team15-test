#include "../gmock-1.6.0/include/gmock/gmock.h"
#include "../Record.h"

#include <string>
#include <vector>
using namespace std;

//14 test cases

vector<string> get_attributes() {
  string attr_array[] = { "Ol' Rock", "56", "Super Senior", "4.0", "2013/01/01" };
  vector<string> attr(attr_array, attr_array + 5);

  return attr;
}

// This is a where a factory or fixture would be helpful?
Record shared_record;
void setup_shared_record() {
  shared_record = Record(get_attributes());
}

/* Test Construction of Records */
TEST(RecordConstructionTest, CreatesEmptyRecord) {
  EXPECT_NO_THROW(Record());
}

TEST(RecordConstructionTest, CreatesPredefinedRecord) {
  vector<string> attributes = get_attributes();
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
TEST(RecordRetrievalTest, RetrievesIntegerAttribute) {
  setup_shared_record();

  EXPECT_EQ("56", shared_record.retreiveRecord(1));
}

TEST(RecordRetrievalTest, RetrievesFloatingAttribute) {
  setup_shared_record();

  EXPECT_EQ("4.0", shared_record.retreiveRecord(3));
}

TEST(RecordRetrievalTest, RetrievesDateAttribute) {
  setup_shared_record();

  EXPECT_EQ("01/01/2013", shared_record.retreiveRecord(4));
}

TEST(RecordRetrievalTest, RetrievesTimeAtrribute) {
  setup_shared_record();

  EXPECT_EQ("13:55:23", shared_record.retreiveRecord(7));
}


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

TEST(RecordModifyTest, ModifiesNegativeIntegerAttribute) {
  setup_shared_record();

  shared_record.modifyRecord(5, "-8");

  EXPECT_EQ("-8", shared_record.retreiveRecord(5));
}

TEST(RecordModifyTest, ModifiesNegativeFloatingPointAttribute) {
  setup_shared_record();

  shared_record.modifyRecord(6, "-6.9");

  EXPECT_EQ("-6.9", shared_record.retreiveRecord(6));
}

TEST(RecordModifyTest, ModifiesDateAttribute) {
  setup_shared_record();

  shared_record.modifyRecord(4, "2013/02/09");

  EXPECT_EQ("2013/02/09", shared_record.retreiveRecord(4));
}

TEST(RecordModifyTest, ModifiesTimeAttribute) {
  setup_shared_record();

  shared_record.modifyRecord(7, "15:25:23");

  EXPECT_EQ("15:25:23", shared_record.retreiveRecord(7));
}

#include "data_loader.h"
#include "inputdata.h"

#include <tuple>
#include <vector>
#include <array>

#include "Table.h"
#include "Database.h"

template<int Num>
struct TableDescriptor {
  typedef array<pair<string, Table::TYPE>, Num> type;
};

static const TableDescriptor<2>::type ACCEPTS = {
  make_pair("placeID", Table::STRING),
  make_pair("payment", Table::STRING)
};

static const TableDescriptor<2>::type CUISINE = {
  make_pair("placeID", Table::STRING),
  make_pair("cuisine", Table::STRING)
};

static const TableDescriptor<3>::type HOURS = {
  make_pair("placeID", Table::STRING),
  make_pair("hours", Table::STRING),
  make_pair("days", Table::STRING)
};

static const TableDescriptor<2>::type PARKING = {
  make_pair("placeID", Table::STRING),
  make_pair("parking_lot", Table::STRING)
};

static const TableDescriptor<21>::type PLACES = {
  make_pair("placeID", Table::STRING),
  make_pair("latitude", Table::FLOAT),
  make_pair("longitude", Table::FLOAT),
  make_pair("geom_meter", Table::STRING),
  make_pair("name", Table::STRING),
  make_pair("address", Table::STRING),
  make_pair("city", Table::STRING),
  make_pair("state", Table::STRING),
  make_pair("country", Table::STRING),
  make_pair("fax", Table::STRING),
  make_pair("zip", Table::STRING),
  make_pair("alcohol", Table::STRING),
  make_pair("smoking_area", Table::STRING),
  make_pair("dress_code", Table::STRING),
  make_pair("accessibility", Table::STRING),
  make_pair("price", Table::STRING),
  make_pair("url", Table::STRING),
  make_pair("ambience", Table::STRING),
  make_pair("franchise", Table::STRING),
  make_pair("area", Table::STRING),
  make_pair("other_services", Table::STRING)
};

static const TableDescriptor<5>::type RATING = {
  make_pair("userID", Table::STRING),
  make_pair("placeID", Table::STRING),
  make_pair("rating", Table::INT),
  make_pair("food_rating", Table::INT),
  make_pair("service_rating", Table::INT)
};

static const TableDescriptor<2>::type USER_CUISINE = {
  make_pair("userID", Table::STRING),
  make_pair("cuisine", Table::STRING)
};

static const TableDescriptor<2>::type USER_PAYMENT = {
  make_pair("userID", Table::STRING),
  make_pair("payment", Table::STRING)
};

static const TableDescriptor<19>::type USER_PROFILE = {
  make_pair("userID", Table::STRING),
  make_pair("latitude", Table::FLOAT),
  make_pair("longitude", Table::FLOAT),
  make_pair("smoker", Table::STRING),
  make_pair("drink_level", Table::STRING),
  make_pair("dress_preference", Table::STRING),
  make_pair("ambience", Table::STRING),
  make_pair("transport", Table::STRING),
  make_pair("martial_status", Table::STRING),
  make_pair("has_children", Table::STRING),
  make_pair("birth_year", Table::INT),
  make_pair("interest", Table::STRING),
  make_pair("personality", Table::STRING),
  make_pair("religion", Table::STRING),
  make_pair("activity", Table::STRING),
  make_pair("color", Table::STRING),
  make_pair("weight", Table::INT),
  make_pair("budget", Table::FLOAT),
  make_pair("height", Table::FLOAT)
};

DataLoader::DataLoader(string data_dir)
  : data_dir_(data_dir)
{
}

DataLoader::~DataLoader() {}

Database* DataLoader::load() {
  db_ = new Database;
  load_table("chefmozaccepts.csv", "PaymentType", ACCEPTS);
  load_table("chefmozcuisine.csv", "Cuisine", CUISINE);
  load_table("chefmozhours.csv", "Hours", HOURS);
  load_table("chefmozparking.csv", "Parking", PARKING);
  load_table("geoplaces2.csv", "Locations", PLACES);
  load_table("rating_final.csv", "Ratings", RATING);
  load_table("usercuisine.csv", "UserCuisine", USER_CUISINE);
  load_table("userpayment.csv", "UserPayment", USER_PAYMENT);
  load_table("userprofile.csv", "UserProfile", USER_PROFILE);
  return db_;
}

template<typename D>
void DataLoader::load_table(string filename, string table_name, const D& descriptor) {
  vector<pair<string, Table::TYPE>> spec(descriptor.begin(), descriptor.end());
  vector<vector<string>> records = read_data(data_dir_+"\\"+filename);
  Table table = Table(spec);
  for (auto record : records)
    table.insert(record);
  db_->addTable(table, table_name);
}

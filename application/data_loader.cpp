#include "data_loader.h"
#include "inputdata.h"

#include <tuple>
#include <vector>
#include <array>

#include "Table.h"
#include "Database.h"

static const DataLoader::TableDescriptor<2>::type ACCEPTS = {
  make_pair("placeID", Table::STRING),
  make_pair("payment", Table::STRING)
};

static const DataLoader::TableDescriptor<2>::type CUISINE = {
  make_pair("placeID", Table::STRING),
  make_pair("cuisine", Table::STRING)
};

static const DataLoader::TableDescriptor<3>::type HOURS = {
  make_pair("placeID", Table::STRING),
  make_pair("hours", Table::STRING),
  make_pair("days", Table::STRING)
};

static const DataLoader::TableDescriptor<2>::type PARKING = {
  make_pair("placeID", Table::STRING),
  make_pair("parking_lot", Table::STRING)
};

static const DataLoader::TableDescriptor<21>::type PLACES = {
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
}

DataLoader::DataLoader(string data_dir) {

}

DataLoader::~DataLoader() {}

Database* DataLoader::load() {
  db_ = new Database;
  load_table("chefmozaccepts.csv", "PaymentType", ACCEPTS);
  load_table("chefmozcuisine.csv", "Cuisine", CUISINE);
  load_table("chefmozhours.csv", "Hours", HOURS);
  load_table("chefmozparking.csv", "Parking", PARKING);
  load_table("geoplaces2.csv", "Locations", PLACES);
  return db_;
}

template<int Num>
void DataLoader::load_table(string filename, string table_name, TableDescriptor<Num>* descriptor) {
  vector<pair<string, Table::TYPE>> spec(descriptor->begin(), descriptor->end());
  vector<Record> records = read_data(filename).record_vector;
  Table table = new Table(spec);
  //for (const Record& record : records)
  //  table->insert(record);
  db_->addTable(table, table_name);
}

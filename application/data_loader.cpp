#include "data_loader.h"
#include "inputdata.h"

#include <tuple>
#include <vector>
#include <array>

#include "Table.h"
#include "Database.h"

static const DataLoader::TableDescriptor<2>::type CM_ACCEPTS = {
  make_pair("placeID", Table::STRING),
  make_pair("Rpayment", Table::STRING)
};

static const DataLoader::TableDescriptor<2>::type CM_CUISINE = {
  make_pair("placeID", Table::STRING),
  make_pair("Rcuisine", Table::STRING)
};

static const DataLoader::TableDescriptor<3>::type CM_HOURS = {
  make_pair("placeID", Table::STRING),
  make_pair("hours", Table::STRING),
  make_pair("days", Table::STRING)
};

static const DataLoader::TableDescriptor<2>::type CM_PARKING = {
  make_pair("placeID", Table::STRING),
  make_pair("parking_lot", Table::STRING)
};

DataLoader::DataLoader(string data_dir) {

}

DataLoader::~DataLoader() {}

Database* DataLoader::load() {
  Database *db = new Database;
  return db;
}

template<int Num>
Table* DataLoader::load_table(string filename, TableDescriptor<Num>* descriptor) {
  vector<pair<string, Table::TYPE>> spec(descriptor->begin(), descriptor->end());
  vector<Record> records = read_data(filename).record_vector;
  Table table = new Table(spec);
  //for (const Record& record : records)
  //  table->insert(record);
  return table;
}

#include "data_loader.h"

#include <tuple>
#include <vector>
#include <array>

#include "Table.h"
#include "Database.h"

template<int Num>
struct TableDescriptor {
  typedef array<tuple<string, Table::TYPE>, Num> type;
};

static const TableDescriptor<2>::type CM_ACCEPTS = {
  make_tuple("placeID", Table::STRING),
  make_tuple("Rpayment", Table::STRING)
};

static const TableDescriptor<2>::type CM_CUISINE = {
  make_tuple("placeID", Table::STRING),
  make_tuple("Rcuisine", Table::STRING)
};

static const TableDescriptor<3>::type CM_HOURS = {
  make_tuple("placeID", Table::STRING),
  make_tuple("hours", Table::STRING),
  make_tuple("days", Table::STRING)
};

static const TableDescriptor<2>::type CM_PARKING = {
  make_tuple("placeID", Table::STRING),
  make_tuple("parking_lot", Table::STRING)
};

DataLoader::DataLoader(string data_dir) {

}

DataLoader::~DataLoader() {}

Database* DataLoader::load() {
  Database *db = new Database;
  return db;
}

#pragma once

#include <string>
#include <array>
#include <utility>
using namespace std;

#include "Table.h"

class Database;

// Class to load data from CSV files into a database.
class DataLoader
{
public:
  DataLoader(string data_dir);
  ~DataLoader();

  Database* load();

private:
  template<typename D>
  void load_table(string filename, string table_name, const D& descriptor);

  Database *db_;
  string data_dir_;
};

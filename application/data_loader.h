#pragma once

#include <string>
#include <array>
#include <utility>
using namespace std;

#include "Table.h"

class Database;

class DataLoader
{
public:
  DataLoader(string data_dir);
  ~DataLoader();

  Database* load();

  template<int Num>
  struct TableDescriptor {
    typedef array<pair<string, Table::TYPE>, Num> type;
  };

private:
  template<int Num>
  Table* load_table(string filename, TableDescriptor<Num>* descriptor);
};

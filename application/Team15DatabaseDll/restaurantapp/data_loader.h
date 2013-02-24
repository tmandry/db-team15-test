#pragma once

#include <string>
using namespace std;

class DataLoader
{
public:
  DataLoader(string data_dir);
  ~DataLoader();

  Database* load();
};

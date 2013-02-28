#pragma once

#include "restaurant_printer.h"

#include <map>
#include <string>
#include <vector>

using namespace std;

class Database;

class Menu
{
public:
  Menu(Database *db);
  virtual ~Menu();

  void run();

  typedef void (*CommandFunc)(RestaurantPrinter printer, vector<string> args);

private:
  RestaurantPrinter printer_;
  map<string, CommandFunc> commands_;
  Database *db_;
};
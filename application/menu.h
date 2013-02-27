#pragma once

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

  typedef void (*CommandFunc)(vector<string> args);

private:
  map<string, CommandFunc> commands_;
  Database *db_;
};


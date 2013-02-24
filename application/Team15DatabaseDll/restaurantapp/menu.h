#pragma once

#include <map>
#include <string>
#include <vector>

using namespace std;

class Menu
{
public:
  Menu();
  virtual ~Menu();

  void run();

  typedef void (*CommandFunc)(vector<string> args);

private:
  map<string, CommandFunc> commands_;
};


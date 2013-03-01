// restaurantapp.cpp : Defines the entry point for the console application.
//

#include <iostream>

#include "menu.h"
#include "inputdata.h"
#include "data_loader.h"

#include "Database.h"

int main()
{
  cout << "Loading database...";
  DataLoader loader("..\\application\\appdata");
  Database *db = loader.load();
  cout << "done.\n";
  cout << "Type `help` to see a list of commands." << endl;

  Menu menu(db);
  menu.run();
	return 0;
}

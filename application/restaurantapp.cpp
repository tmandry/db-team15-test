// restaurantapp.cpp : Defines the entry point for the console application.
//

#include <iostream>

#include "menu.h"
#include "inputdata.h"
#include "data_loader.h"

#include "Database.h"

int main()
{
  cout << "Loading database...\n";
  DataLoader loader("..\\application\\appdata");
  Database *db = loader.load();

  Menu menu(db);
  menu.run();
	return 0;
}

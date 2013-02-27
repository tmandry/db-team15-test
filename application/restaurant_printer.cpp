#include "restaurant_printer.h"
#include "bprinter/table_printer.h"

RestaurantPrinter::RestaurantPrinter(Database *database) {
  database_ = database;
}

RestaurantPrinter::~RestaurantPrinter() {}

void RestaurantPrinter::set_database(Database *database) {
  database_ = database;
}

Database* RestaurantPrinter::get_database() {
  return database_;
}

void RestaurantPrinter::print_customer(string customer_id) {
  print_table(database_->query("*", "userprofile", "userID = " + customer_id));
}

void print_table(Table table) {
  TableIterator it(table);
  it.first();
  Record customer = it.getRecord();

  bprinter::TablePrinter tp(&std::cout);

  for (auto name_type : table.attributes()) {
    tp.AddColumn(name_type.first, 25);
  }
  tp.PrintHeader();

  for (int i = 0; i < table.attributes().size(); i++) {
    tp << customer.retrieve(i);
  }

  tp << bprinter::endl();
  tp.PrintFooter();
}
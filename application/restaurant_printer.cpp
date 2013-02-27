#include "restaurant_printer.h"
#include "bprinter/table_printer.h"
#include <functional>

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
  print_table("Customer Information: " + customer_id, database_->query("*", "userprofile", "userID = " + customer_id));
}

void RestaurantPrinter::print_customer_ratings(string customer_id) {
  print_table("Customer Ratings: " + customer_id, database_->query("*", "rating_final", "userID = " + customer_id));
}

void RestaurantPrinter::print_customer_restaurants(string customer_id) {

}

void RestaurantPrinter::print_customers_with_at_least_budget(string minimum_budget) {
  print_table("Customers With Minimum Budget: " + minimum_budget, database_->query("*", "userprofile", "budget >= " + minimum_budget));
}

void RestaurantPrinter::print_restaurant(string restaurant_name) {
  print_table("Restaurant Information: " + restaurant_name, database_->query("*", "geoplaces2", "name = '" + restaurant_name + "'"));
}

void RestaurantPrinter::print_restaurant_ratings(string restaurant_name) {
  // may be able to use a join here (wasn't sure how to select just the
  // attributes on the rating_final table and join in one query)
  Table restaurant = database_->query("placeID", "geoplaces2", "name = '" + restaurant_name + "'");
  TableIterator it(restaurant);
  it.first();
  print_table("Ratings for Restaurant: " + restaurant_name, database_->query("*", "rating_final", "placeID = " + it.getRecord().retrieve(0)));
}

void RestaurantPrinter::print_restaurant_customers(string restaurant_name) {

}

void RestaurantPrinter::print_restaurants_with_cuisine(string cuisine) {
  Table restaurants = database_->query("placeID", "chefmozcuisine", "Rcuisine = '" + cuisine + "'");

  Table results = combine_restaurant_tables(restaurants);

  print_table("Restaurants With Cuisine: " + cuisine, results);
}

void RestaurantPrinter::print_restaurants_that_accept(string payment_type) {
  Table restaurants = database_->query("placeID", "chefmozaccepts", "Rpayment = '" + payment_type + "'");

  Table results = combine_restaurant_tables(restaurants);

  print_table("Restaurants With Payment Type: " + payment_type, results);
}

void RestaurantPrinter::print_restaurants_with_at_least_average_rating(float minimum_rating) {

}

// Prints all columns of a table to stdout
void RestaurantPrinter::print_table(string title, Table table) {
  cout << "*******\t" << title << "\t*******" << endl << endl;

  bprinter::TablePrinter tp(&std::cout);
  for (auto name_type : table.attributes()) {
    tp.AddColumn(name_type.first, 25);
  }
  tp.PrintHeader();

  // prints a row of all attributes in the record
  auto print_record = [&] (Record &record) {
    for (int i = 0; i < record.size(); i++) {
      tp << record.retrieve(i);
    }
  };

  for_each_record(table, print_record);

  tp << bprinter::endl();
  tp.PrintFooter();

  cout << "# of results:\t" << table.size() << endl << endl;
}

// Helper due to strange implementation of TableIterator. Maps a function over
// every record in a table.
void RestaurantPrinter::for_each_record(Table &table, function<void (Record&)> procedure) {
  TableIterator it(table);

  // handling of the first record
  it.first();
  procedure(it.getRecord());

  // iterator through the rest
  for (int i = 1; i < table.size(); i++) {
    it.next();
    procedure(it.getRecord());
  }
}

// Gives us a way to emulate SQL's 'IN' operator. Takes a table that only
// has one column (placeID). It then gets the full information for each
// restaurant and combines them into one table.
Table RestaurantPrinter::combine_restaurant_tables(Table placeIDs) {
  // create a vector of all the information for each restaurant
  vector<Table> restaurants_vector;
  auto push = [&] (Record &record) {
    restaurants_vector.push_back(database_->query("*", "geoplaces2", "placeID = " + record.retrieve(0)));
  };
  for_each_record(placeIDs, push);

  // combine tables into one
  Table results(restaurants_vector[0].attributes());
  for (Table restaurant : restaurants_vector) {
    auto insert_into_results = [&] (Record &record) {
      vector<string> attributes;
      for (int i = 0; i < record.size(); i++) {
        attributes.push_back(record.retrieve(i));
      }
      results.insert(attributes);
    };
    for_each_record(restaurant, insert_into_results);
  }

  return results;
}
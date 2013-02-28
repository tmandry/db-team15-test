#include "restaurant_printer.h"
#include "bprinter/table_printer.h"
#include "TableIterator.h"
#include <functional>
#include <fstream>

RestaurantPrinter::RestaurantPrinter(Database *database, string filename)
  : database_(database), filename_(filename)
{
}

RestaurantPrinter::~RestaurantPrinter() {
}

void RestaurantPrinter::set_database(Database *database) {
  database_ = database;
}

Database* RestaurantPrinter::get_database() {
  return database_;
}

void RestaurantPrinter::print_customer(string customer_id) {
  print_table("Customer Information: " + customer_id, database_->query("*", "UserProfile", "userID = '" + customer_id + "'"));
}

void RestaurantPrinter::print_customer_ratings(string customer_id) {
  print_table("Customer Ratings: " + customer_id, database_->query("*", "Ratings", "userID = '" + customer_id + "'"));
}

void RestaurantPrinter::print_customers_with_at_least_budget(string minimum_budget) {
  Table results;
  if (minimum_budget == "low") {
    results = database_->query("*", "UserProfile", "budget = 'low' OR budget = 'medium' OR budget = 'high'");
  } else if (minimum_budget == "medium") {
    results = database_->query("*", "UserProfile", "budget = 'medium' OR budget = 'high'");
  } else {
    results = database_->query("*", "UserProfile", "budget = 'high'");
  }
  
  print_table("Customers With Minimum Budget: " + minimum_budget, results);
}

void RestaurantPrinter::print_restaurant(string restaurant_name) {
  print_table("Restaurant Information: " + restaurant_name, database_->query("*", "Locations", "name = '" + restaurant_name + "'"));
}

void RestaurantPrinter::print_restaurant_ratings(string restaurant_name) {
  // may be able to use a join here (wasn't sure how to select just the
  // attributes on the Ratings table and join in one database_->query)
  Table restaurant = database_->query("placeID", "Locations", "name = '" + restaurant_name + "'");
  TableIterator it(restaurant);
  it.first();
  print_table("Ratings for Restaurant: " + restaurant_name, database_->query("userID, rating, food_rating, service_rating", "Ratings", "placeID = '" + it.getRecord().retrieve(0) + "'"));
}

void RestaurantPrinter::print_restaurant_hours(string restaurant_name) {
  Table restaurant = database_->query("placeID", "Locations", "name = '" + restaurant_name + "'");
  TableIterator it(restaurant);
  it.first();
  print_table("Hours for Restaurant: " + restaurant_name, database_->query("hours, days", "Hours", "placeID = '" + it.getRecord().retrieve(0) + "'"));
}

void RestaurantPrinter::print_restaurants_with_cuisine(string cuisine) {
  Table restaurants = database_->query("placeID", "Cuisine", "cuisine = '" + cuisine + "'");

  Table results = lookup_and_combine_restaurant_tables(restaurants);

  print_table("Restaurants With Cuisine: " + cuisine, results);
}

void RestaurantPrinter::print_restaurants_that_accept(string payment_type) {
  Table restaurants = database_->query("placeID", "PaymentType", "payment = '" + payment_type + "'");

  Table results = lookup_and_combine_restaurant_tables(restaurants);

  print_table("Restaurants With Payment Type: " + payment_type, results);
}

void RestaurantPrinter::print_restaurants_with_at_least_average_rating(float minimum_rating) {
  Table restaurants = database_->query("*", "Locations", "");

  // create a table with the same structure as Ratings
  Table restaurants_with_min_average(database_->query("*", "Ratings", ""));

  auto find_average = [&] (Record &record) {
    Table ratings = database_->query("*", "Ratings", "placeID = '" + record.retrieve(0) + "'");
    float average = ratings.sum("rating") / ratings.count("rating");
    // if it meets the minimum we want to insert the FIRST instance of the rating
    // into the table so that the placeID is present once, and can be passed to
    // lookup_and_combine_restaurant_tables()
    if (average > minimum_rating) {
      TableIterator it(ratings);
      it.first();
      Record record = it.getRecord();
      vector<string> attributes;
      for (int i = 0; i < record.size(); i++) {
        attributes.push_back(record.retrieve(i));
      }
      restaurants_with_min_average.insert(attributes);
    }
  };
  for_each_record(restaurants, find_average);

  print_table("Restaurants With Minimum Average Rating", lookup_and_combine_restaurant_tables(restaurants_with_min_average));
}

void RestaurantPrinter::print_all_restaurant_customer_combinations() {
  Table restaurants = database_->query("name", "Locations", "");
  Table customers = database_->query("userID", "UserProfile", "");

  print_table("All Restaurant-Customer Combinations", restaurants.crossJoin(customers));
}

// Prints all columns of a table to file
void RestaurantPrinter::print_table(string title, Table table) {
  ofstream output;
  output.open(filename_);

  cout << "*******\t" << title << "\t*******" << endl << endl;
  output << "*******\t" << title << "\t*******" << endl << endl;

  bprinter::TablePrinter tp(&output);
  for (auto name_type : table.attributes())
    tp.AddColumn(name_type.first, 15);
  tp.PrintHeader();

  // prints a row of all attributes in the record
  auto print_record = [&] (Record &record) {
    for (int i = 0; i < record.size(); i++)
      tp << record.retrieve(i);
  };

  for_each_record(table, print_record);

  tp << bprinter::endl();
  tp.PrintFooter();

  cout << "# of results:\t" << table.size() << endl << endl;
  output << "# of results:\t" << table.size() << endl << endl;

  output.close();
}

// Helper due to strange implementation of TableIterator. Maps a function over
// every record in a table.
void RestaurantPrinter::for_each_record(Table &table, function<void (Record&)> procedure) {
  TableIterator it(table);

  // handling of the first record

  if (table.size() > 0) {
    it.first();
    procedure(it.getRecord());
     
    // iterate through the rest
    for (int i = 1; i < table.size(); i++) {
      it.next();
      procedure(it.getRecord());
    }
  }


}

// Gives us a way to emulate SQL's 'IN' operator. Takes a table where the first
// column is placeID. It then gets the full information for each restaurant
// in one table.
Table RestaurantPrinter::lookup_and_combine_restaurant_tables(Table placeIDs) {
  string where_clause;
  auto build_where_clause = [&] (Record &record) {
    if (where_clause != "")
      where_clause += " OR ";
    where_clause += "placeID = " + record.retrieve(0);
  };
  for_each_record(placeIDs, build_where_clause);

  return database_->query("*", "Locations", where_clause);
}

#include "restaurant_printer.h"
#include "bprinter/table_printer.h"
#include "TableIterator.h"
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
  print_table("Customer Information: " + customer_id, query("*", "userprofile", "userID = " + customer_id));
}

void RestaurantPrinter::print_customer_ratings(string customer_id) {
  print_table("Customer Ratings: " + customer_id, query("*", "rating_final", "userID = " + customer_id));
}

void RestaurantPrinter::print_customers_with_at_least_budget(string minimum_budget) {
  print_table("Customers With Minimum Budget: " + minimum_budget, query("*", "userprofile", "budget >= " + minimum_budget));
}

void RestaurantPrinter::print_restaurant(string restaurant_name) {
  print_table("Restaurant Information: " + restaurant_name, query("*", "geoplaces2", "name = '" + restaurant_name + "'"));
}

void RestaurantPrinter::print_restaurant_ratings(string restaurant_name) {
  // may be able to use a join here (wasn't sure how to select just the
  // attributes on the rating_final table and join in one query)
  Table restaurant = query("placeID", "geoplaces2", "name = '" + restaurant_name + "'");
  TableIterator it(restaurant);
  it.first();
  print_table("Ratings for Restaurant: " + restaurant_name, query("*", "rating_final", "placeID = " + it.getRecord().retrieve(0)));
}

void RestaurantPrinter::print_restaurants_with_cuisine(string cuisine) {
  Table restaurants = query("placeID", "chefmozcuisine", "Rcuisine = '" + cuisine + "'");

  Table results = lookup_and_combine_restaurant_tables(restaurants);

  print_table("Restaurants With Cuisine: " + cuisine, results);
}

void RestaurantPrinter::print_restaurants_that_accept(string payment_type) {
  Table restaurants = query("placeID", "chefmozaccepts", "Rpayment = '" + payment_type + "'");

  Table results = lookup_and_combine_restaurant_tables(restaurants);

  print_table("Restaurants With Payment Type: " + payment_type, results);
}

void RestaurantPrinter::print_restaurants_with_at_least_average_rating(float minimum_rating) {
  Table restaurants = query("*", "geoplaces2", "");

  // create a table with the same structure as rating_final
  Table restaurants_with_min_average(query("*", "rating_final", ""));

  auto find_average = [&] (Record &record) {
    Table ratings = query("*", "rating_final", "placeID = " + record.retrieve(0));
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

// Prints all columns of a table to stdout
void RestaurantPrinter::print_table(string title, Table table) {
  cout << "*******\t" << title << "\t*******" << endl << endl;

  bprinter::TablePrinter tp(&std::cout);
  for (auto name_type : table.attributes())
    tp.AddColumn(name_type.first, 25);
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
}

Table RestaurantPrinter::query(string select, string from, string where) {
  return database_->query(select, from, where);
}

// Helper due to strange implementation of TableIterator. Maps a function over
// every record in a table.
void RestaurantPrinter::for_each_record(Table &table, function<void (Record&)> procedure) {
  TableIterator it(table);

  // handling of the first record
  it.first();
  procedure(it.getRecord());

  // iterate through the rest
  for (int i = 1; i < table.size(); i++) {
    it.next();
    procedure(it.getRecord());
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

  return query("*", "geoplaces2", where_clause);
}

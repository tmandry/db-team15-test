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

string RestaurantPrinter::id_for_restaurant(string restaurant_name) {
  Table restaurant = database_->query("placeID", "Locations", "name = '" + restaurant_name + "'");
  TableIterator it(restaurant);
  it.first();
  return it.getRecord().retrieve(0);
}

void RestaurantPrinter::print_restaurant(string restaurant_name) {
  print_table("Restaurant Information: " + restaurant_name, database_->query("*", "Locations", "name = '" + restaurant_name + "'"));
}

void RestaurantPrinter::print_restaurant_ratings(string restaurant_name) {
  print_table("Ratings for Restaurant: " + restaurant_name, database_->query("userID, rating, food_rating, service_rating", "Ratings", "placeID = '" + id_for_restaurant(restaurant_name) + "'"));
}

void RestaurantPrinter::print_restaurant_average_rating(string restaurant_name) {
  Table results = database_->query("userID, placeID, rating, food_rating, service_rating", "Ratings", "placeID = '" + id_for_restaurant(restaurant_name) + "'");
  cout << "Average ratings for restaurant: " << restaurant_name << endl;
  cout << "Overall: " << results.sum("rating")/results.count("rating") << endl;
  cout << "Food:    " << results.sum("food_rating")/results.count("food_rating") << endl;
  cout << "Service: " << results.sum("service_rating")/results.count("service_rating") << endl << endl;
}

void RestaurantPrinter::print_restaurant_hours(string restaurant_name) {
  print_table("Hours for Restaurant: " + restaurant_name, database_->query("hours, days", "Hours", "placeID = '" + id_for_restaurant(restaurant_name) + "'"));
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

void RestaurantPrinter::print_average_customer_rating(string userid) {
  Table results = database_->query("userID, placeID, rating, food_rating, service_rating", "Ratings", "placeID = '" + id_for_restaurant(userid) + "'");
  Table average_customer_rating(database_->query("*", "Ratings", ""));

  auto average_rating = [&] (Record &records) {
    Table ratings = database_->query("*", "Ratings", "placeID = '" + records.retrieve(0) + "'");
    float average = ratings.sum("rating") / ratings.count("rating");
    TableIterator it(ratings);
    it.first();
    Record record = it.getRecord();
    vector<string> attributes;
    for (int i = 0; i < record.size(); i++) {
      attributes.push_back(record.retrieve(i));
    }
    average_customer_rating.insert(attributes);
  };

  for_each_record(results, average_rating);

  print_table("Average Rating for Customer", lookup_and_combine_restaurant_tables(average_customer_rating));
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
void RestaurantPrinter::print_table(string title, Table &table) {
  ofstream output;
  output.open(filename_, fstream::app);

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
// in one table. NOTE: Due to the inefficiency of Database::query, any queries involving
// this function will take quiet a bit of time (up to 5 minutes)
Table RestaurantPrinter::lookup_and_combine_restaurant_tables(Table &placeIDs) {
  // create a vector of all the information for each restaurant
  vector<Table> restaurants_vector;
  auto push = [&] (Record &record) {
    Table query = database_->query("*", "Locations", "placeID = " + record.retrieve(0));
    if (query.size() == 0) {
      vector<string> attr;
      attr.push_back(record.retrieve(0));
      for (int i = attr.size(); i < query.attributes().size(); i++)
        attr.push_back(string());
      query.insert(attr);
    }
    restaurants_vector.push_back(query);
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
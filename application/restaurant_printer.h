#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <functional>
#include "Database.h"

// Takes parameters to make queries and print out results to file
class RestaurantPrinter {
public:
  RestaurantPrinter(Database *database, string filename);
  ~RestaurantPrinter();

  void set_database(Database *database);
  Database* get_database();

  // There are many queries we could come up with
  void print_customer(string customer_id);
  void print_customer_ratings(string customer_id);
  void print_customers_with_at_least_budget(string minimum_budget);

  void print_restaurant(string restaurant_name);
  void print_restaurant_ratings(string restaraunt_name);
  void print_restaurant_average_rating(string restaurant_name);
  void print_restaurant_hours(string restaurant_name);
  void print_restaurants_with_cuisine(string cuisine);
  void print_restaurants_that_accept(string payment_type);
  void print_restaurants_with_at_least_average_rating(float minimum_rating);

  void print_all_restaurant_customer_combinations();
private:

  void print_table(string title, Table table);
  void for_each_record(Table &table, function<void (Record&)> procedure);
  Table lookup_and_combine_restaurant_tables(Table placeIDs);

  string id_for_restaurant(string restaraunt_name);

  Database* database_;
  string filename_;
};

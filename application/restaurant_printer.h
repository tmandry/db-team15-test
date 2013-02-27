#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <functional>
#include "Database.h"

// Takes parameters to make queries and print out results to stdout
class RestaurantPrinter {
public:
  RestaurantPrinter(Database *database);
  ~RestaurantPrinter();

  void set_database(Database *database);
  Database* get_database();

  // There are many queries we could come up with
  void print_customer(string customer_id);
  void print_customer_ratings(string customer_id);
  void print_customers_with_at_least_budget(string minimum_budget);

  void print_restaurant(string restaurant_name);
  void print_restaurant_ratings(string restaraunt_name);
  void print_restaurants_with_cuisine(string cuisine);
  void print_restaurants_that_accept(string payment_type);
  void print_restaurants_with_at_least_average_rating(float minimum_rating);
private:
  Table query(string select, string from, string where);
  void print_table(string title, Table table);
  void for_each_record(Table &table, function<void (Record&)> procedure);
  Table lookup_and_combine_restaurant_tables(Table placeIDs);

  Database *database_;
};
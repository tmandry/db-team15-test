#pragma once

#include <string>
#include <iostream>
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
  void print_customer_restaurants(string customer_id);
  void print_customers_with_at_least_budget(string minimum_budget);

  void print_restaurant(string restaurant_name);
  void print_restaurant_ratings(string restaraunt_name);
  void print_restaurant_customers(string restaurant_name);
  void print_restaurants_with_cuisine(string cuisine);
  void print_restaurants_that_accept(string payment_type);
  void print_restaurants_with_at_least_average_rating(string minimum_rating);
private:
  void print_table(Table table);
  Database *database_;
};
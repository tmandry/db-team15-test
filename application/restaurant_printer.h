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

  // Get and set the database pointer
  void set_database(Database *database);
  Database* get_database();

  // Display all customers
  void print_all_customers();
  // Display all restaurants
  void print_all_restaurants();
  // Display general info on a customer
  void print_customer(string customer_id);
  // Display all ratings by a customer
  void print_customer_ratings(string customer_id);
  // Display all customers with budget of at least min-budget
  void print_customers_with_at_least_budget(string minimum_budget);
  // Display general info on a restaurant
  void print_restaurant(string restaurant_name);
  // Display all ratings for a restaurant
  void print_restaurant_ratings(string restaraunt_name);
  // Display average ratings for a restaurant
  void print_restaurant_average_rating(string restaurant_name);
  // Display a customer's rating for a particular restaurant
  void print_customer_restaurant_rating(string customer_id, string restaurant_name);
  // Display restaurant hours
  void print_restaurant_hours(string restaurant_name);
  // Display all restaurants offering the given cuisine
  void print_restaurants_with_cuisine(string cuisine);
  // Display all restaurants that accept the given payment-type
  void print_restaurants_that_accept(string payment_type);
  // Display all restaurants that have an average overall rating of at least min-rating
  void print_restaurants_with_at_least_average_rating(float minimum_rating);
  // Display average ratings given by a customer
  void print_average_customer_rating(string restaraunt_name);
private:
  void print_table(string title, Table &table);
  void for_each_record(Table &table, function<void (Record&)> procedure);
  Table lookup_and_combine_restaurant_tables(Table &placeIDs, unsigned index_of_placeid);
  string id_for_restaurant(string restaraunt_name);

  Database* database_;
  string filename_;
};

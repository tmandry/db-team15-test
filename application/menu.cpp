#include "menu.h"
#include "string_tokenizer.h"
#include "bprinter/table_printer.h"

#include <tuple>
#include <vector>
#include <array>
#include <iostream>

using namespace std;

// Helper functions to connect menu commands to RestaurantPrinter functions.
void customers(RestaurantPrinter printer, vector<string> args) { printer.print_all_customers(); }
void customer_info(RestaurantPrinter printer, vector<string> args) { printer.print_customer(args[0]); }
void customer_ratings(RestaurantPrinter printer, vector<string> args) { printer.print_customer_ratings(args[0]); }
void customer_budgets(RestaurantPrinter printer, vector<string> args) { printer.print_customers_with_at_least_budget(args[0]); }
void restaurants(RestaurantPrinter printer, vector<string> args) { printer.print_all_restaurants(); }
void restaurant_info(RestaurantPrinter printer, vector<string> args) { printer.print_restaurant(args[0]); }
void restaurant_ratings(RestaurantPrinter printer, vector<string> args) { printer.print_restaurant_ratings(args[0]); }
void restaurant_average_rating(RestaurantPrinter printer, vector<string> args) { printer.print_restaurant_average_rating(args[0]); }
void restaurant_hours(RestaurantPrinter printer, vector<string> args) { printer.print_restaurant_hours(args[0]); }
void cuisine(RestaurantPrinter printer, vector<string> args) { printer.print_restaurants_with_cuisine(args[0]); }
void payment(RestaurantPrinter printer, vector<string> args) { printer.print_restaurants_that_accept(args[0]); }
void average_rating(RestaurantPrinter printer, vector<string> args) { printer.print_restaurants_with_at_least_average_rating(atof(args[0].c_str())); }
void average_customer_rating(RestaurantPrinter printer, vector<string> args) { printer.print_average_customer_rating(args[0]); }
void help(RestaurantPrinter printer, vector<string> args);
void customer_restaurant_rating(RestaurantPrinter printer, vector<string> args) { 
  if (args.size() != 2)
    cout << "2 arguments expected\n";
  else
    printer.print_customer_restaurant_rating(args[0], args[1]); 
}

// Define commands here, each with a tuple of the command name, the arguments
// it takes, a description, and a function that takes a vector<string> of
// arguments and implements the command.
typedef tuple<string, string, string, Menu::CommandFunc> CommandDescriptor;
// You must update the number of commands in the line below.
static const array<CommandDescriptor, 15> COMMANDS = {
  make_tuple("customers",        "",               "Display all customers", customers),
  make_tuple("customer",         "<customer-id>", "Display general info on a customer",  customer_info),
  make_tuple("customer-ratings", "<customer-id>", "Display all ratings by a customer", customer_ratings),
  make_tuple("customer-restaurant-rating", "<customer-id>, <restaurant-name>", "Display a customer's rating for a particular restaurant", customer_restaurant_rating),
  make_tuple("average-customer-rating", "<customer-id>", "Display average ratings given by a customer", average_customer_rating),
  make_tuple("budgets",          "<min-budget>",  "Display all customers with budget of at least min-budget", customer_budgets),
  make_tuple("restaurants",      "",              "Display all restaurants", restaurants),
  make_tuple("restaurant",         "<restaurant-name>", "Display general info on a restaurant", restaurant_info),
  make_tuple("restaurant-ratings", "<restaurant-name>", "Display all ratings for a restaurant", restaurant_ratings),
  make_tuple("restaurant-average", "<restaurant-name>", "Display average ratings for a restaurant", restaurant_average_rating),
  make_tuple("restaurant-hours",   "<restaurant-name>", "Display restaurant hours", restaurant_hours),
  make_tuple("average-rating-at-least", "<min-rating>", "Display all restaurants that have an average overall rating of at least min-rating", average_rating),
  make_tuple("payment",            "<payment-type>", "Display all restaurants that accept the given payment-type", payment),
  make_tuple("cuisine",            "<cuisine-type>", "Display all restaurants offering the given cuisine", cuisine),
  make_tuple("help", "", "Print this list", help)
};

// Prints a list of commands.
void help(RestaurantPrinter printer, vector<string> args) {
  cout << "You can use these commands:\n\n";

  for (CommandDescriptor cmd : COMMANDS) {
    cout << " " << get<0>(cmd) << " " << get<1>(cmd) << "\n";
    cout << "   " << get<2>(cmd) << "\n";
  }

  cout << "\nType `exit` to exit." << endl;
}

Menu::Menu(Database *db)
  : db_(db), printer_(RestaurantPrinter(db, "output.txt"))
{
  // build map of commands
  for (const CommandDescriptor& cmd : COMMANDS) {
    commands_[get<0>(cmd)] = get<3>(cmd);
  }
}

Menu::~Menu() {
}

// Main loop where the menu prompt executes.
void Menu::run() {
  while (true) {
    cout << "> ";

    // Read the command
    string line;
    getline(cin, line);
    StringTokenizer tok(line);

    // Parse the command
    string command = tok.next_token();
    tok.set_delim(',');
    vector<string> args;
    while (!tok.at_end()) {
      args.push_back(tok.next_token());
    }

    if (command == "exit") break;

    // Execute the command
    auto cmd_it = commands_.find(command);
    if (cmd_it == commands_.end()) {
      cout << "Command not found." << endl;
    } else {
      (*cmd_it->second)(printer_, args);
    }
  }
}

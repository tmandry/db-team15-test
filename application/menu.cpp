#include "menu.h"
#include "string_tokenizer.h"

#include <tuple>
#include <vector>
#include <array>
#include <iostream>

using namespace std;

void my_func(RestaurantPrinter printer, vector<string> args) {cout << "Doing stuff!\n";}
void customer_info(RestaurantPrinter printer, vector<string> args) { printer.print_customer(args[0]); };
void customer_ratings(RestaurantPrinter printer, vector<string> args) { printer.print_customer_ratings(args[0]); };
void customer_budgets(RestaurantPrinter printer, vector<string> args) { printer.print_customers_with_at_least_budget(args[0]); };
void restaurant_info(RestaurantPrinter printer, vector<string> args) { printer.print_restaurant(args[0]); };
void restaurant_ratings(RestaurantPrinter printer, vector<string> args) { printer.print_restaurant_ratings(args[0]); };
void restaurant_hours(RestaurantPrinter printer, vector<string> args) { printer.print_restaurant_hours(args[0]); };
void cuisine(RestaurantPrinter printer, vector<string> args) { printer.print_restaurants_with_cuisine(args[0]); };
void payment(RestaurantPrinter printer, vector<string> args) { printer.print_restaurants_that_accept(args[0]); };
void average_rating(RestaurantPrinter printer, vector<string> args) { printer.print_restaurants_with_at_least_average_rating(atof(args[0].c_str())); };
void rest_cust_combos(RestaurantPrinter printer, vector<string> args) { printer.print_all_restaurant_customer_combinations(); };

// Define commands here, each with a tuple of the command name and a function
// that takes a vector<string> of arguments and implements the command.
typedef tuple<string, Menu::CommandFunc> CommandDescriptor;
// You must update the number of commands in the line below.
static const array<CommandDescriptor, 11> COMMANDS = {
  make_tuple("dostuff", my_func),
  make_tuple("customer", customer_info),
  make_tuple("customer ratings", customer_ratings),
  make_tuple("budgets", customer_budgets),
  make_tuple("restaurant", restaurant_info),
  make_tuple("restaurant ratings", restaurant_ratings),
  make_tuple("restaurant hours", restaurant_hours),
  make_tuple("cuisine", cuisine),
  make_tuple("payment", payment),
  make_tuple("average rating", average_rating),
  make_tuple("restaurant customer combinations", rest_cust_combos)
};

Menu::Menu(Database *db)
  : db_(db), printer_(RestaurantPrinter(db, "output.txt"))
{
  // build map of commands
  for (const CommandDescriptor& cmd : COMMANDS) {
    commands_[get<0>(cmd)] = get<1>(cmd);
  }
}

Menu::~Menu() {
}

void Menu::run() {
  while (true) {
    cout << "> ";

    string line;
    getline(cin, line);
    StringTokenizer tok(line);

    string command = tok.next_token();
    tok.set_delim(',');
    vector<string> args;
    while (!tok.at_end()) {
      args.push_back(tok.next_token());
    }

    if (command == "exit") break;

    auto cmd_it = commands_.find(command);
    if (cmd_it == commands_.end()) {
      cout << "Command not found." << endl;
    } else {
      (*cmd_it->second)(printer_, args);
    }
  }
}
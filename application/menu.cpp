#include "menu.h"
#include "string_tokenizer.h"

#include <tuple>
#include <vector>
#include <array>
#include <iostream>

using namespace std;

void my_func(RestaurantPrinter printer, vector<string> args) {cout << "Doing stuff!\n";}
void customer_info(RestaurantPrinter printer, vector<string> args) { printer.print_customer(args[0]); };

// Define commands here, each with a tuple of the command name and a function
// that takes a vector<string> of arguments and implements the command.
typedef tuple<string, Menu::CommandFunc> CommandDescriptor;
// You must update the number of commands in the line below.
static const array<CommandDescriptor, 2> COMMANDS = {
  make_tuple("dostuff", my_func),
  make_tuple("customer", customer_info)
};

Menu::Menu(Database *db)
  : db_(db), printer_(RestaurantPrinter(db))
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
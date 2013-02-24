#include "menu.h"

#include <tuple>
#include <vector>
#include <array>
#include <iostream>

using namespace std;

void my_func(vector<string> args) {cout << "Doing stuff!\n";}

// Define commands here, each with a tuple of the command name and a function
// that takes a vector<string> of arguments and implements the command.
// You must update the number of commands in the line below.
static const array<tuple<string, Menu::CommandFunc>, 1> COMMANDS = {
  make_tuple("dostuff", my_func)
};

class StringTokenizer
{
public:
  StringTokenizer(string str);
  ~StringTokenizer() {}

  string next_token();
  bool at_end() const;

  void set_delim(char d) { delim_ = d; }

private:
  string str_;
  size_t pos_;
  char delim_;
};

Menu::Menu()
{
  // build map of commands
  for (auto cmd : COMMANDS) {
    commands_[get<0>(cmd)] = get<1>(cmd);
  }
}

Menu::~Menu()
{
}

void Menu::run()
{
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
      (*cmd_it->second)(args);
    }
  }
}

StringTokenizer::StringTokenizer(string str)
  : str_(str),
    pos_(str.find_first_not_of(' ')),
    delim_(' ')
{
}

// Returns the next token, skipping consecutive spaces.
string StringTokenizer::next_token()
{
  if (pos_ >= str_.size()) return "";

  size_t next_space = str_.find(delim_, pos_);
  string token = str_.substr(pos_, next_space - pos_);

  pos_ = (next_space == string::npos) ? string::npos : next_space + 1;
  while (!at_end() && str_[pos_] == ' ')
    ++pos_;

  return token;
}

bool StringTokenizer::at_end() const
{
  return pos_ == string::npos || pos_ >= str_.size();
}

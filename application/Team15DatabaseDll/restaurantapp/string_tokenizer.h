#pragma once

#include <string>
using namespace std;

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

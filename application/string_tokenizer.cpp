#include "string_tokenizer.h"

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

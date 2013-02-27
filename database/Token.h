#ifndef __TOKEN__TEAM15_
#define __TOKEN__TEAM15_

#include "DllExportInclude.h"
#include <string>

using namespace std;

class DBDLL_API Token {
public:
    char kind;        // what kind of token
	float value;
    string name;     // for numbers: a value  
    Token(char ch)    // make a Token from a char
        :kind(ch), value(0) { }   
	Token(char ch, float val)
		:kind(ch), value(val) { }
    Token(char ch, string val)     // make a Token from a char and a double
        :kind(ch), name(val) { }
};

#endif

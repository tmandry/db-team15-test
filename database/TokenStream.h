#ifndef __TOKENSTREAM__TEAM15_
#define __TOKENSTREAM__TEAM15_

#include "DllExportInclude.h"
#include "Token.h"
#include <string>
#include <iostream>
#include <sstream>

using namespace std;

class DBDLL_API TokenStream
{
public: 
	TokenStream(stringstream &stream);   // make a Token_stream that reads from cin
    Token get();      // get a Token (get() is defined elsewhere)
    void putback(Token t);    // put a Token back
	bool good() { return ss.good(); }
private:
    bool full;        // is there a Token in the buffer?
    Token buffer;     // here is where we keep a Token put back using putback()
	stringstream &ss;
};

#endif

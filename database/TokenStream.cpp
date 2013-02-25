#include "Token.h"
#include "TokenStream.h"
#include <string>
#include <iostream>
#include <sstream>

using namespace std;

TokenStream::TokenStream(stringstream &stream):full(false), buffer(0), ss(stream)   // no Token in buffer
{
	
}

void TokenStream::putback(Token t)
{
    if (full)
	{
		throw "putback() into a full buffer";
	}
    buffer = t;       // copy t to buffer
    full = true;      // buffer is now full
}

Token TokenStream::get()
{
    if (full)
	{       // do we already have a Token ready?
        // remove token from buffer
        full=false;
        return buffer;
    } 

    char ch;
    ss >> ch;    // note that >> skips whitespace (space, newline, tab, etc.)

    switch (ch)
	{
	case '(': case ')': case '+': case '*': case '/': case '=': 
        return Token(ch);        // let each character represent itself
	case '-':
		{
			char next;
			ss >> next;
			switch (next)
			{
			case '.':
			case '0': case '1': case '2': case '3': case '4':
			case '5': case '6': case '7': case '8': case '9':
				{
					ss.putback(next);
					ss.putback(ch);
					float val;
					ss >> val;
					return Token('8', val);
				}
			default:
				return Token(ch);
			}
		}
	case '.':
    case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
        {    
            ss.putback(ch);         // put digit back into the input ss
            float val;
            ss >> val;              // read a floating-point number
            return Token('8', val);   // let '8' represent "a number"
        }
	case '>':
		{
			ss.putback(ch);
			string name;
			ss >> name;
			if(name.compare(">=") == 0)
				return Token('g', "equal");
			else
				return Token('g');
		}
	case '<':
		{
			ss.putback(ch);
			string name;
			ss >> name;
			if(name.compare("<=") == 0)
				return Token('l', "equal");
			else
				return Token('l');
		}
	case '!':
		{
			ss.putback(ch);
			string name;
			ss >> name;
			if(name.compare("!=") == 0)
				return Token('n', "equal");
			else
			{
				while(name.size() > 0)
				{
					ss.unget();
					name.pop_back();
				}
				ss >> ch;
			}
		}

	case '\'':
		{
			ss >> std::noskipws;
			char c;
			string name = "";
			ss >> noskipws >> c;
			while(c != '\'')
			{
				name += c;
				ss >> noskipws >> c;
			}
			ss >> skipws;
			return Token('s', name);
		}
	case 'A': case 'O':
		{
			ss.putback(ch);
			string name;
			ss >> name;
			if(name.compare("AND") == 0)
			{
				return Token('A');
			}
			else if(name.compare("OR") == 0)
			{
				return Token('O');
			}
			else
			{
				while(name.size() > 0)
				{
					ss.unget();
					name.pop_back();
				}
				ss >> ch;
			}
		}
    default:
		{
			ss.putback(ch);
			string name;
			ss >> name;
			return Token('v', name);
		}
    }
}
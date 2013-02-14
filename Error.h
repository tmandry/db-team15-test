#ifndef __ERROR__TEAM15_
#define __ERROR__TEAM15_

#include <string>
#include <exception>

using namespace std;

class Error : public exception {
public:
	Error(const string& message);
};

#endif // __ERROR__TEAM15_

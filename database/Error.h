#ifndef __ERROR__TEAM15_
#define __ERROR__TEAM15_

#include "DllExportInclude.h"
#include <string>
#include <exception>

using namespace std;

class DBDLL_API Error : public exception {
public:
	Error(const string& message);

	virtual const char* what() const throw();

private:

	char* errorMessage;
};

#endif // __ERROR__TEAM15_

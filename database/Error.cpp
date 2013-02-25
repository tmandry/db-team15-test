#include "DllExportInclude.h"
#include "Error.h"
#include <string>

using namespace std;

Error::Error(const string& message)
{
	errorMessage = (char*)message.c_str();
}

const char* Error::what() const throw()
{
	return errorMessage;
}

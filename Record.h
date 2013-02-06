#ifndef __RECORD__TEAM15_
#define __RECORD__TEAM15_

#include <vector>
#include <string>
#include <map>
using namespace std;

class Record
{
public:
	/**
	* Creates a Record from the inputed vector of strings.
	*/
	Record(vector<string> newEntry) {};

	/**
	* Retrieves an entry from the Record
	* @param index Specific index in container of desired entry
	* @return The desired entry is returned as a string
	*/
	string retrieveRecord(int index);

	/**
	* Modifies an entry in the Record
	* @param index Specific place in container of entry to be modified
	* @param newAttr New string to go in entry
	* @return Nothing is returned
	*/
	void modifyRecord(int index, string newAttr);
	

private:
	vector<string> entries;

};


#endif

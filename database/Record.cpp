#include "DllExportInclude.h"
#include "Record.h"
#include <vector>
#include <string>
#include <map>
using namespace std;

Record::Record() 
{
	entries = vector<string>();
}

Record::Record(vector<string> newEntry)
{
	entries = vector<string>();
	for (int i = 0; i < newEntry.size(); i++)
	{
		entries.push_back(newEntry[i]);
	}
}

string Record::retrieveRecord(int index)
{ 
	return entries[index]; 
}

string Record::retrieve(int index)
{
	return entries[index]; 
}

string Record::retrieveConst(int index) const
{
	return entries[index];
}

void Record::modifyRecord(int index, string newAttr) 
{
	entries[index] = newAttr;
}

void Record::modify(int index, string newAttr) 
{
	entries[index] = newAttr;
}

int Record::size() const
{
	return entries.size();
}
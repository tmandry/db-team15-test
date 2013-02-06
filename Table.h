#ifndef __TABLE__TEAM15_
#define __TABLE__TEAM15_

#include "Record.h"
#include <string>
#include <vector>
#include <map>

using namespace std;  

class Table
{
public:

	/**
	 * Default constructor to create a Table
	 */
	Table() {}

	/**
	 * Creates a table with the given attribute names and types
	 * @param attrNames A list of strings containing attribute names
	 * @param attrTypes A list of strings containing attribute types
	 */
	Table(vector<string> attrNames, vector<string> attrTypes) {}

	/**
	 * Add an attribute to the table
	 * @param attrName The name of the new attribute
	 * @param attrType The type of the new attribute
	 * @return Returns true on success, false on failure
	 */
	bool add(string attrName, string attrType);

	/**
	 * Removes an attribute from a table
	 * @param attrName The name of the attribute to remove
	 * @return Returns true on success, false on failure
	 */
	bool deleteAttribute(string attrName);

	/**
	 * Inserts a new record into the table
	 * @param newRecords The list of records to add, one for each attribute
	 * @return Returns true on success, false on failure
	 */
	bool insert(vector<string> newRecords);

	/**
	 * Get the attributes of a table
	 * @return Returns vector<string> of the table attributes
	 */
	vector<string> attributes();

	/**
	 * Get the size of the table
	 * @return Returns the number of entries in the table
	 */
	int size();

	/**
	 * Renames an attribute of the table
	 * @param attrName The attribute to be changed
	 * @param newName The new name of the attribute
	 * @return Returns true on success, false on failure
	 */
	bool rename(string attrName, string newName);

	/**
	 * Cross join two tables
	 * @param t1 The first table to cross
	 * @param t2 The second table to cross
	 * @return Returns the cross of the two input tables
	 */
	Table crossJoin(Table t1, Table t2);

	/**
	 * Sum an attribute
	 * @param attrName The name of the attribute to sum
	 * @return Returns the sum of the records of that attribute
	 */
	float sum(string attrName);

	/**
	 * Count the number of entries of a particular attribute
	 * @param attrName The name of the attribute to count
	 * @return Returns the number non-NULL entries of that attribute
	 */
	int count(string attrName);

	/**
	 * Find the minimum entry of a particular attribute
	 * @param attrName The name of the attribute to find the minimum of
	 * @return Returns the minimum entry of the attribute
	 */
	float min(string attrName);

	/**
	 * Find the maximum entry of a particular attribute
	 * @param attrName The name of the attribute to find the maximum of
	 * @return Returns the maximum entry of the attribute
	 */
	float max(string attrName);

private:
	vector<Record> records; /**< A vector containing all the records of the table */
	vector<Record>::iterator iter; /**< An iterator to traverse the records */
};

#endif	// __TABLE__TEAM15_

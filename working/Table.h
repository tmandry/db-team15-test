#ifndef __TABLE__TEAM15_
#define __TABLE__TEAM15_

#include "DllExportInclude.h"
#include "Record.h"
#include <utility>
#include <string>
#include <vector>
#include <map>

using namespace std;  

class DBDLL_API Table
{
public:
	friend class TableIterator;
	/**
	 * Enum containing all the valid input types for the Table.
	 */
	enum TYPE {
		INT,
		FLOAT,
		STRING,
		DATE
	};

	/**
	 * Default constructor to create a Table
	 */
	Table();

	/**
	 * Creates a table with the given attribute names and types
	 * @param attributes A map<string, TYPE> containing the names of the attributes and Types
	 */
	Table(vector<pair<string, TYPE>> attributes);

	/**
	 * Creates a table with the given attribute names and types
	 * @param attrNames A list of strings containing attribute names
	 * @param attrTypes A list of strings containing attribute types
	 */
	Table(vector<string> attrNames, vector<TYPE> attrTypes);

	/**
	 * Add an attribute to the table
	 * @param attrName The name of the new attribute
	 * @param attrType The type of the new attribute
	 * @return Returns true on success, false on failure
	 */
	bool add(string attrName, TYPE attrType);

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
	 * @return Returns map<string, TYPE> of the table attribute names and types
	 */
	vector<pair<string, TYPE>> attributes();

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
	 * @param table The table to cross with
	 * @return Returns the cross of the two tables
	 */
	Table crossJoin(Table table);

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
	vector<pair<string, TYPE> > attrNameType;
};

class DBDLL_API TableIterator
{
public:
	/**
	 * Creates a table iterator with the given Table
	 * @param table The table you want to iterate through\
	 */
	TableIterator(Table& table);

	void first();	/**< Move to the first entry in the table */

	/**
	 * Move to the next entry in the table
	 * @return Returns true on success, false if there are no more records left in the table
	 */
	bool next();

	/**
	 * Get the record at the current position of the iterator
	 * @return The record at the current position of the iterator
	 */
	Record getRecord();
private:
	vector<Record>::const_iterator begin;
	vector<Record>::const_iterator position;
	vector<Record>::const_iterator end;
};

#endif	// __TABLE__TEAM15_

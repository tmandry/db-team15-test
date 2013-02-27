#ifndef __TABLEITERATOR__TEAM15_
#define __TABLEITERATOR__TEAM15_

#include "DllExportInclude.h"
#include "Table.h"
#include <string>
#include <vector>

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
	Record& getRecord();
private:
	Table *table;
	vector<Record>::const_iterator begin;
	vector<Record>::const_iterator position;
	vector<Record>::const_iterator end;
};

#endif

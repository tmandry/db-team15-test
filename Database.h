#ifdef DBDLL_EXPORTS
#define DBDLL_API __declspec(dllexport) 
#else
#define DBDLL_API __declspec(dllimport) 
#endif

#ifndef __DATABASE__TEAM15_
#define __DATABASE__TEAM15_

#include "Table.h"
#include <string>
#include <vector>

using namespace std;

class DBDLL_API Database 
{
public:
	
	/**
	* Default Constructor to create a Database.
	*/
	Database() {};

	
	/**
	 * Add a Table to the Database.
	 * @param table The Table to be added to the Database
	 * @param tableName Name of the Table to be added
	 * @return true on success, false on failure
	 */
	bool addTable(Table table, string tableName);

	/**
	 * Drops a Table from the Database
	 * @param tableName Name of the Table to be dropped from the Database
	 * @return true on success, false if not found
	 */
	bool dropTable(string tableName);

	/**
	 * Returns a vector of all the table names currently in the Database.
	 * @return vector<string> of Table names
	 */
	vector<string> getTableNames();

	/**
	 * Returns a vector of Tables currently in the Database.
	 * @return vector<Table> of all Tables
	 */
	vector<Table> getTables();

	/**
	 * Queries the Database and retrieves the wanted data
	 * @param selectString The list of attributes you want to retrieve
	 * @param fromString Table to select from
	 * @param whereString Conditions for retrieved data
	 * @return Table with all the selected data
	 */
	Table query(string selectString, string fromString, string whereString);

	/**
	 * Delete data from a Table
	 * @param selectString The list of attributes you want to meet for deletion
	 * @param fromString Table to delete from
	 * @param whereString Conditions for deleting data
	 * @return True on success, False on error
	 */
	bool deleteQuery(string selectString, string fromString, string whereString);

private:

	vector<Table> tables;
};

#endif

#ifndef __DATABASE__TEAM15_
#define __DATABASE__TEAM15_

#include "DllExportInclude.h"
#include "Table.h"
#include "TokenStream.h"
#include <string>
#include <vector>

using namespace std;

class DBDLL_API Database
{
public:

	/**
	* Default Constructor to create a Database.
	*/
	Database();


	/**
	 * Add a Table to the Database.
	 * @param table The Table to be added to the Database
	 * @param tableName Name of the Table to be added
	 * @return true on success, false on failure
	 */
	bool addTable(Table &table, string &tableName);
	bool addTable(Table &table, char tableName[])
	{
		return addTable(table, string(tableName));
	}

	/**
	 * Drops a Table from the Database
	 * @param tableName Name of the Table to be dropped from the Database
	 * @return true on success, false if not found
	 */
	bool dropTable(string &tableName);
	bool dropTable(char tableName[])
	{
		return dropTable(string(tableName));
	}

	/**
	 * Returns a vector of all the table names currently in the Database.
	 * @return vector<string> of Table names
	 */
	vector<string>& getTableNames();

	/**
	 * Returns a vector of Tables currently in the Database.
	 * @return vector<Table> of all Tables
	 */
	vector<Table>& getTables();

	/**
	 * Queries the Database and retrieves the wanted data
	 * @param selectString The list of attributes you want to retrieve
	 * @param fromString Table to select from
	 * @param whereString Conditions for retrieved data
	 * @return Table with all the selected data
	 */
	Table& query(string &selectString, string &fromString, string &whereString);
	Table& query(char selectString[], char fromString[], char whereString[])
	{
		return query(string(selectString), string(fromString), string(whereString));
	}

	/**
	 * Delete data from a Table
	 * @param selectString The list of attributes you want to meet for deletion
	 * @param fromString Table to delete from
	 * @param whereString Conditions for deleting data
	 * @return True on success, False on error
	 */
	bool deleteQuery(string &selectString, string &fromString, string &whereString);
	bool deleteQuery(char selectString[], char fromString[], char whereString[])
	{
		return deleteQuery(string(selectString), string(fromString), string(whereString));
	}

private:

	vector<Record> newTableRecords(TokenStream &ts, vector<pair<string, Table::TYPE>> attribs, vector<Record> originalRecords);
	vector<Record> term(TokenStream &ts, vector<pair<string, Table::TYPE>> attribs, vector<Record> originalRecords);
	vector<Record> primary(TokenStream &ts, vector<pair<string, Table::TYPE>> attribs, vector<Record> originalRecords);
	vector<Record> combineVectors(vector<Record> left, vector<Record> right, bool and);
	vector<Record> filterString(vector<pair<string, Table::TYPE>> attribs, vector<Record> oldRecords, string column, string value, bool greater, bool less, bool equal);
	vector<Record> filterFloats(vector<pair<string, Table::TYPE>> attribs, vector<Record> oldRecords, string column, float value, bool greater, bool less, bool equal);
	vector<string> splitString(char delim, const string s);
	bool sameRecord(Record left, Record right);
	vector<string> recordToVector(Record rec);
	vector<Record> orderRecords(vector<Record> oldRecords, vector<Record> newRecords);


	vector<pair<string, Table> > tables;
};


#endif

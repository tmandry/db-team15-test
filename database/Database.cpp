#include "DllExportInclude.h"
#include "Table.h"
#include "Database.h"
#include "Error.h"
#include "Token.h"
#include "TokenStream.h"
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <algorithm>

Database::Database()
{
	tables = vector<pair<string, Table> >();
}

bool Database::addTable(Table table, string tableName) 
{ 
	for(int i=0;i<tables.size();i++)
	{
		if(tables[i].first == tableName)
		{
			throw Error("Table already exists");
		}
	}
	tables.push_back(pair<string, Table>(tableName, table));
	return true;
}

bool Database::dropTable(string tablename)
{
	for(int i=0;i<tables.size();i++)
	{
		if(tables[i].first == tablename)
		{
			tables.erase(tables.begin()+i);
			return true;
		}
	}
	return false;
}

vector<string> Database::getTableNames()
{
	vector<string> names = vector<string>();
	for(int i=0;i<tables.size();i++){
		names.push_back(tables[i].first);
	}

	return names;
}

vector<Table> Database::getTables()
{
	vector<Table> t = vector<Table>();
	for(int i=0;i<tables.size();i++)
	{
		t.push_back(tables[i].second);
	}
	return t;
}

Table Database::query(string selectString, string fromString, string whereString)
{
	Table oldTable;
	bool found = false;
	for(int i=0;i<tables.size();i++)
	{
		if(tables[i].first.compare(fromString) == 0)
		{
			oldTable = tables[i].second;
			found = true;
		}
	}
	if(!found)
	{
		throw Error("Invalid Database in Query String");
	}

	vector<pair<string, Table::TYPE>> attribs = oldTable.attributes();
	Table returnTable = Table(attribs);
	vector<Record> records = vector<Record>();
	vector<Record> oldRecords = vector<Record>();
	if(oldTable.size() > 1)
	{
		TableIterator it = TableIterator(oldTable);
		oldRecords.push_back(it.getRecord());
		while(it.next())
		{
			oldRecords.push_back(it.getRecord());
		}
		
		stringstream ss(whereString);
		TokenStream ts(ss);
		if (whereString.compare("") != 0)
		{
			records = newTableRecords(ts, attribs, oldRecords);
		}
		else
		{
			records = oldRecords;
		}
	}
	records = orderRecords(oldRecords, records);
	for(int i=0;i<records.size();i++)
	{
		vector<string> rec = vector<string>();
		for(int j=0;j<records[i].size();j++)
		{
			rec.push_back(records[i].retrieve(j));
		}
		returnTable.insert(rec);
	}

	if(selectString.compare("*") == 0 || selectString.compare("") == 0)
	{
		return returnTable;
	}
	else
	{
		vector<string> parts = splitString(',', selectString);
		bool rem = true;
		for(int i=0;i<attribs.size();i++)
		{
			for(int j=0;j<parts.size();j++){
				string trimmed = parts[j];
				remove(trimmed.begin(), trimmed.end(), ' ');
				if(attribs[i].first.compare(trimmed) == 0)
				{
					rem = false;
				}
			}
			if(rem)
			{
				returnTable.deleteAttribute(attribs[i].first);
			}
		}
		return returnTable;
	}
}

bool Database::deleteQuery(string selectString, string fromString, string whereString)
{
	Table oldTable;
	bool found = false;
	int tableLoc = 0;
	for(int i=0;i<tables.size();i++)
	{
		if(tables[i].first.compare(fromString) == 0)
		{
			oldTable = tables[i].second;
			tableLoc = i;
			found = true;
		}
	}
	if(!found)
	{
		throw Error("Invalid Database in Query String");
	}

	vector<pair<string, Table::TYPE>> attribs = oldTable.attributes();
	Table returnTable = Table(attribs);
	vector<Record> records = vector<Record>();
	vector<Record> oldRecords = vector<Record>();
	if(oldTable.size() > 1)
	{
		TableIterator it = TableIterator(oldTable);
		oldRecords.push_back(it.getRecord());
		while(it.next())
		{
			oldRecords.push_back(it.getRecord());
		}
		
		stringstream ss(whereString);
		TokenStream ts(ss);
		if (whereString.compare("") != 0)
		{
			records = newTableRecords(ts, attribs, oldRecords);
		}
		else
		{
			records = oldRecords;
		}
	}
	records = orderRecords(oldRecords, records);
	for(int i=0;i<records.size();i++)
	{
		vector<string> rec = vector<string>();
		for(int j=0;j<records[i].size();j++)
		{
			rec.push_back(records[i].retrieve(j));
		}
		returnTable.insert(rec);
	}
	Table newTable = Table(attribs);
	if(oldTable.size() > 0 && returnTable.size() > 0)
	{
		TableIterator ot = TableIterator(oldTable);
		TableIterator dt = TableIterator(returnTable);
		Record otRecord, dtRecord;
		bool add = true;
		do
		{
			otRecord = ot.getRecord();
			do
			{
				dtRecord = dt.getRecord();
				if(sameRecord(otRecord, dtRecord))
				{
					add = false;
				}
			}while(dt.next());
			dt.first();

			if(add)
			{
				newTable.insert(recordToVector(otRecord));
			}
			add = true;
		}while(ot.next());
		tables[tableLoc].second = newTable;
		return true;
	}
	return true;
	
}

vector<Record> Database::newTableRecords(TokenStream &ts, vector<pair<string, Table::TYPE>> attribs, vector<Record> originalRecords)
{
	vector<Record> newRecords = originalRecords;
	while(ts.good())
	{
		vector<Record> left = term(ts, attribs, originalRecords);
		Token t = ts.get();
		switch(t.kind)
		{
		case 'A':
			newRecords = combineVectors(left, newTableRecords(ts, attribs, originalRecords), true);
			return newRecords;
		case 'O':
			newRecords = combineVectors(left, newTableRecords(ts, attribs, originalRecords), false);
			return newRecords;
		default:
			ts.putback(t);
			return left;
		}

	}
	return newRecords;
}

vector<Record> Database::term(TokenStream &ts, vector<pair<string, Table::TYPE>> attribs, vector<Record> originalRecords)
{
	vector<Record> left = primary(ts, attribs, originalRecords);
	Token t = ts.get();
	switch(t.kind)
	{
	case 'v':
		{
			string vname = t.name;
			Token compare = ts.get();
			if(compare.kind != '=' && compare.kind != 'g' && compare.kind != 'l' && compare.kind != 'n')
			{
				throw Error("Expected compare type (=, >, >=, <, <=, !=)");
			}
			t = ts.get();
			switch(t.kind)
			{
			case '8':
				{
					float d = t.value;
					if(compare.kind == 'g' && compare.name.compare("equal") == 0)
						return filterFloats(attribs, originalRecords, vname, d, true, false, true);
					else if(compare.kind == 'g' && compare.name.compare("equal") != 0)
						return filterFloats(attribs, originalRecords, vname, d, true, false, false);
					else if(compare.kind == 'l' && compare.name.compare("equal") == 0)
						return filterFloats(attribs, originalRecords, vname, d, false, true, true);
					else if(compare.kind == 'l' && compare.name.compare("equal") != 0)
						return filterFloats(attribs, originalRecords, vname, d, false, true, false);
					else if(compare.kind == '=')
						return filterFloats(attribs, originalRecords, vname, d, false, false, true);
					else
						return filterFloats(attribs, originalRecords, vname, d, false, false, false);

				}
			case 's':
				{
					if(compare.kind == 'g' && compare.name.compare("equal") == 0)
						return filterString(attribs, originalRecords, vname, t.name, true, false, true);
					else if(compare.kind == 'g' && compare.name.compare("equal") != 0)
						return filterString(attribs, originalRecords, vname, t.name, true, false, false);
					else if(compare.kind == 'l' && compare.name.compare("equal") == 0)
						return filterString(attribs, originalRecords, vname, t.name, false, true, true);
					else if(compare.kind == 'l' && compare.name.compare("equal") != 0)
						return filterString(attribs, originalRecords, vname, t.name, false, true, false);
					else if(compare.kind == '=')
						return filterString(attribs, originalRecords, vname, t.name, false, false, true);
					else
						return filterString(attribs, originalRecords, vname, t.name, false, false, false);
				}
			default:
				throw Error("Invalid Syntax in query");
			}
		}
	case '8':
		{
			float d = t.value;
			Token compare = ts.get();
			if(compare.kind != '=' && compare.kind != 'g' && compare.kind != 'l' && compare.kind != 'n')
			{
				throw Error("Expected compare type (=, >, >=, <, <=, !=)");
			}
			t = ts.get();
			if(t.kind != 'v')
			{
				throw Error("Expected a variable in query");
			}
			string vname = t.name;
			if(compare.kind == 'g' && compare.name.compare("equal") == 0)
				return filterFloats(attribs, originalRecords, vname, d, true, false, true);
			else if(compare.kind == 'g' && compare.name.compare("equal") != 0)
				return filterFloats(attribs, originalRecords, vname, d, true, false, false);
			else if(compare.kind == 'l' && compare.name.compare("equal") == 0)
				return filterFloats(attribs, originalRecords, vname, d, false, true, true);
			else if(compare.kind == 'l' && compare.name.compare("equal") != 0)
				return filterFloats(attribs, originalRecords, vname, d, false, true, false);
			else if(compare.kind == '=')
				return filterFloats(attribs, originalRecords, vname, d, false, false, true);
			else
				return filterFloats(attribs, originalRecords, vname, d, false, false, false);
		}
	case 's':
		{
			string value = t.name;
			Token compare = ts.get();
			if(compare.kind != '=' && compare.kind != 'n')
			{
				throw Error("Expected compare type (=, >, >=, <, <=, !=)");
			}
			t = ts.get();
			if(compare.kind == 'g' && compare.name.compare("equal") == 0)
				return filterString(attribs, originalRecords, t.name, value, true, false, true);
			else if(compare.kind == 'g' && compare.name.compare("equal") != 0)
				return filterString(attribs, originalRecords, t.name, value, true, false, false);
			else if(compare.kind == 'l' && compare.name.compare("equal") == 0)
				return filterString(attribs, originalRecords, t.name, value, false, true, true);
			else if(compare.kind == 'l' && compare.name.compare("equal") != 0)
				return filterString(attribs, originalRecords, t.name, value, false, true, false);
			else if(compare.kind == '=')
				return filterString(attribs, originalRecords, t.name, value, false, false, true);
			else
				return filterString(attribs, originalRecords, t.name, value, false, false, false);
		}
	case ')':
		{
			ts.putback(t);
			return left;
		}
	default:
		throw Error("Something went wrong");
	}
}

vector<Record> Database::primary(TokenStream &ts, vector<pair<string, Table::TYPE>> attribs, vector<Record> originalRecords)
{
	Token t = ts.get();
	switch(t.kind)
	{
	case '(':
		{
			vector<Record> e = newTableRecords(ts, attribs, originalRecords);
			t = ts.get();
			if(t.kind != ')')
				throw Error("Expected ')' in query");
			ts.putback(t);
			return e;
		}
	default:
		ts.putback(t);
		return originalRecords;
	}
}

vector<Record> Database::combineVectors(vector<Record> left, vector<Record> right, bool and)
{
	bool add = true;
	vector<Record> combined = vector<Record>();
	if(and)
	{
	for(int i=0;i<left.size();i++)
	{
			for(int j=0;j<right.size();j++)
			{
				if(sameRecord(left[i], right[j]))
				{
					combined.push_back(left[i]);
				}
			}
		}
	}
	else 
		{
		for(int i=0;i<left.size();i++)
			{
			for(int j=0;j<right.size();j++)
				{
				if(sameRecord(left[i], right[j]))
				{
					add = false;
				}
			}
			if(add)
			{
				combined.push_back(left[i]);
			}
			add = true;
		}
		for(int i=0;i<right.size();i++)
		{
			combined.push_back(right[i]);
		}
	}

	return combined;
}

vector<Record> Database::filterString(vector<pair<string, Table::TYPE>> attribs, vector<Record> oldRecords, string column, string value, bool greater, bool less, bool equal)
{
	int columnNumber = 0;
	for(int i=0;i<attribs.size();i++)
	{
		if(attribs[i].first.compare(column) == 0)
		{
			columnNumber = i;
			break;
		}
	}
	Record temp;
	vector<Record> newRecords = vector<Record>();
	for(int i=0;i<oldRecords.size();i++)
	{
		temp = oldRecords[i];
		string compare = temp.retrieve(columnNumber);
		if(greater && equal)
		{
			if(compare >= value)
			{
				newRecords.push_back(temp);
			}
		}
		else if(greater)
		{
			if(compare > value)
			{
				newRecords.push_back(temp);
			}
		} 
		else if(less && equal)
		{
			if(compare <= value)
			{
				newRecords.push_back(temp);
			}
		}
		else if(less)
		{
			if (compare < value)
			{
				newRecords.push_back(temp);
			}
		}
		else if(equal)
		{
			if(compare.compare(value) == 0)
			{
				newRecords.push_back(temp);
			}
		}
		else
		{
			if(compare.compare(value) != 0)
			{
				newRecords.push_back(temp);
			}

		}
	}
	return newRecords;
}

vector<Record> Database::filterFloats(vector<pair<string, Table::TYPE>> attribs, vector<Record> oldRecords, string column, float value, bool greater, bool less, bool equal)
{
	int columnNumber = 0;
	for(int i=0;i<attribs.size();i++)
	{
		if(attribs[i].first.compare(column) == 0)
		{
			columnNumber = i;
			break;
		}
	}
	Record temp;
	vector<Record> newRecords = vector<Record>();
	for(int i=0;i<oldRecords.size();i++)
	{
		temp = oldRecords[i];
		float data = atof(temp.retrieve(columnNumber).c_str());
		if(greater && equal)
		{
			if(data >= value)
			{
				newRecords.push_back(temp);
			}
		}
		else if(greater)
		{
			if(data > value)
			{
				newRecords.push_back(temp);
			}
		} 
		else if(less && equal)
		{
			if(data <= value)
			{
				newRecords.push_back(temp);
			}
		}
		else if(less)
		{
			if (data < value)
			{
				newRecords.push_back(temp);
			}
		}
		else if(equal)
		{
			if(data == value)
			{
				newRecords.push_back(temp);
			}
		}
		else
		{
			if(data != value)
			{
				newRecords.push_back(temp);
			}

		}
	}
	return newRecords;
}

vector<string> Database::splitString(char delim, const string s)
{
	string str = s;

	vector<string> elems = vector<string>();
	int prev = 0;
	int pos = s.find(delim);
	while(pos != string::npos){
		string sub = s.substr(prev, pos-prev);
		elems.push_back(sub);
		prev = pos+1;
		pos = s.find(delim, prev+1);
	}
	string sub = s.substr(prev);
	elems.push_back(sub);
	return elems;
}

bool Database::sameRecord(Record left, Record right)
{
	if(left.size() != right.size())
		return false;

	for(int i=0;i<left.size();i++)
	{
		if(left.retrieve(i).compare(right.retrieve(i)) != 0)
		{
			return false;
		}
	}
	return true;
}

vector<string> Database::recordToVector(Record rec)
{
	vector<string> data = vector<string>();
	for(int i=0;i<rec.size();i++)
	{
		data.push_back(rec.retrieve(i));
	}
	return data;
}

vector<Record> Database::orderRecords(vector<Record> oldRecords, vector<Record> newRecords)
{
	vector<Record> returnRecords = vector<Record>();
	for(int i=0;i<oldRecords.size();i++)
	{
		for(int j=0;j<newRecords.size();j++)
		{
			if(sameRecord(oldRecords[i], newRecords[j]))
			{
				returnRecords.push_back(newRecords[j]);
			}
		}
	}
	return returnRecords;
}

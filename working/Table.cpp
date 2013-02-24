#include "DllExportInclude.h"
#include "Record.h"
#include "Table.h"
#include "Error.h"
#include <string>
#include <vector>
#include <map>


Table::Table()
{
	records = vector<Record>();
	attrNameType = vector<pair<string, TYPE> >();
}

Table::Table(vector<pair<string, TYPE> > attributes)
{
	records = vector<Record>();
	attrNameType = vector<pair<string, TYPE> >();
	attrNameType.reserve(attributes.size());
	attrNameType.insert(attrNameType.end(), attributes.begin(), attributes.end());
}

Table::Table(vector<string> attrNames, vector<TYPE> attrTypes)
{
	if (attrNames.size() != attrTypes.size())
		throw Error("Size of attrNames and attrTypes are not equal");
	records = vector<Record>();
	attrNameType = vector<pair<string, TYPE> >();
	attrNameType.reserve(attrNames.size());
	for (int i = 0; i < attrNames.size(); i++)
		attrNameType.push_back(make_pair(attrNames[i], attrTypes[i]));
}


bool Table::add(string attrName, Table::TYPE attrType)
{
	int size = attrNameType.size();
	attrNameType.push_back(make_pair(attrName, attrType));
	if (attrNameType.size() != size + 1)
		return false;
	return true;
}

bool Table::deleteAttribute(string attrName)
{
	bool found = false;
	for (int i = 0; i < attrNameType.size(); i++)
	{
		if (attrNameType[i].first == attrName)
		{
			attrNameType.erase(attrNameType.begin() + i);
			found = true;
			break;
		}
	}
	return found;
}

bool Table::insert(vector<string> newRecords)
{
	if (newRecords.size() != attrNameType.size())
		return false;
	records.push_back(Record(newRecords));
	return true;
}

vector<pair<string, Table::TYPE>> Table::attributes()
{
	return attrNameType;
}

int Table::size()
{
	return records.size();
}

bool Table::rename(string attrName, string newName)
{
	bool found = false;
	for (int i = 0; i < attrNameType.size(); i++)
	{
		if (attrNameType[i].first == attrName)
		{
			attrNameType[i].first = newName;
			found = true;
		}
	}
	return found;
}

Table Table::crossJoin(Table table)
{
	vector<pair<string, Table::TYPE> > newAttributes;
	vector<pair<string, Table::TYPE> > tableAttributes = table.attributes();
	newAttributes.reserve(attrNameType.size() + tableAttributes.size());
	newAttributes.insert(newAttributes.end(), attrNameType.begin(), attrNameType.end());
	newAttributes.insert(newAttributes.end(), tableAttributes.begin(), tableAttributes.end());
	Table newTable(newAttributes);

	int size = attrNameType.size();
	int crossSize = tableAttributes.size();
	
	TableIterator crossIter(table);

	for (int i = 0; i < records.size(); i++)
	{
		crossIter.first();
		Record currRecord = records[i];
		do
		{
			vector<string> record;
			record.reserve(size + crossSize);
			for (int k = 0; k < size; k++)
				record.push_back(currRecord.retrieve(k));
			if (table.size() > 0)
			{
				Record crossRecord = crossIter.getRecord();
				for (int k = 0; k < size; k++)
					record.push_back(crossRecord.retrieve(k));
			}
			newTable.insert(record);
		} while (crossIter.next());
	}
	return newTable;
}

float Table::sum(string attrName)
{
	int recPos = 0;
	while (attrNameType[recPos].first != attrName)
	{
		recPos++;
		if (recPos == attrNameType.size())
			throw Error("Attribute does not exist");
	}
	float sum = 0.0;
	for (int i = 0; i < size(); i++)
	{
		string value = records[i].retrieve(recPos);
		if (value != "NULL")
			sum += atof(records[i].retrieve(recPos).c_str());
	}
	return sum;
}

int Table::count(string attrName)
{
	int recPos = 0;
	while (attrNameType[recPos].first != attrName)
	{
		recPos++;
		if (recPos == attrNameType.size())
			throw Error("Attribute does not exist");
	}
	int count = 0;
	for (int i = 0; i < size(); i++)
	{
		string value = records[i].retrieve(recPos);
		if (value != "NULL")
			count++;
	}
	return count;
}

float Table::min(string attrName)
{
	int recPos = 0;
	while (attrNameType[recPos].first != attrName)
	{
		recPos++;
		if (recPos == attrNameType.size())
			throw Error("Attribute does not exist");
	}
	float min = 0;
	int i = 0;
	while (records[i].retrieve(recPos) == "NULL")
	{
		i++;
		if (i == size())
			throw Error("No non-NULL attribute values");
	}
	min = atof(records[i].retrieve(recPos).c_str());
	for (++i; i < size(); i++)
	{
		string value = records[i].retrieve(recPos);
		if (value != "NULL")
		{
			float num = atof(value.c_str());
			if (num < min)
				min = num;
		}
	}
	return min;
}

float Table::max(string attrName)
{
	int recPos = 0;
	while (attrNameType[recPos].first != attrName)
	{
		recPos++;
		if (recPos == attrNameType.size())
			throw Error("Attribute does not exist");
	}
	float max = 0;
	int i = 0;
	while (records[i].retrieve(recPos) == "NULL")
	{
		i++;
		if (i == size())
			throw Error("No non-NULL attribute values");
	}
	max = atof(records[i].retrieve(recPos).c_str());
	for (++i; i < size(); i++)
	{
		string value = records[i].retrieve(recPos);
		if (value != "NULL")
		{
			float num = atof(value.c_str());
			if (num > max)
				max = num;
		}
	}
	return max;
}

TableIterator::TableIterator(Table& table)
{
	begin = table.records.begin();
	position = begin;
	end = table.records.end();
}

void TableIterator::first()
{
	position = begin;
}

bool TableIterator::next()
{
	if (end == begin)
		return false;
	if (position + 1 == end)
		return false;
	position++;
	return true;
}

Record TableIterator::getRecord()
{
	return *position;
}
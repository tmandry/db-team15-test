#include <iostream>
#include <fstream>
#include <vector>
#include "string_tokenizer.h"
#include "Record.h"

using namespace std;

/**
* Struct for returning a vector of records and the column names
* @return A vector of records
* @return a vector of field names
*/
struct ReturnRecord
{
  vector<Record> record_vector;
  vector<string> fields;
};

/**
* Reads in data from a .CSV file and creates a Record
* from each line of data
*
* @param filename Name of the .CSV file to be opened and read in
* @return A struct ReturnRecord
*/
vector< vector<string> > read_data(string filename);

#include "inputdata.h"

using namespace std; 

returnRecord read_data(string filename)
{
  returnRecord record; 
	ifstream data_file;
	string line;
	data_file.open(filename);
	vector<Record> record_vector;
  bool first_line = true;

	// look through each line
	while(getline(data_file, line)) {
		vector<string> fields;
		StringTokenizer tokenized_line(line); 
		tokenized_line.set_delim(',');
		while(!tokenized_line.at_end()) { // push tokens from each line into vector
			string field = tokenized_line.next_token();
			fields.push_back(field);
		}
		Record new_record(fields);
    if(first_line) {
      record.fields = fields; // return the column labels
    }
		record.record_vector.push_back(new_record);
    first_line = false;
	}
	return record;
}
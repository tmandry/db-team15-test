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
	while (getline(data_file, line)) {
		vector<string> fields;
		StringTokenizer tokenizer(line); 
		tokenizer.set_delim(',');

		while (!tokenizer.at_end()) { // push tokens from each line into vector
			string field = tokenizer.next_token();
			fields.push_back(field);
		}

    if (first_line) {
      record.fields = fields; // return the column labels
      first_line = false;
    } else {
      Record new_record(fields);
      record.record_vector.push_back(new_record);
    }
	}

	return record;
}
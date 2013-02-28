#include "inputdata.h"

using namespace std;

vector< vector<string> > read_data(string filename)
{
  ifstream data_file;
	string line;
	data_file.open(filename);
	vector< vector<string> > record_vector;
  bool first_line = true;

	// look through each line
	while (getline(data_file, line)) {
		vector<string> fields;
		StringTokenizer tokenizer(line);
		tokenizer.set_delim(',');
    // push tokens from each line into vector
		while (!tokenizer.at_end()) { 
			string field = tokenizer.next_token();
			fields.push_back(field);
		}
    // skip the first line
    if (first_line) {
      first_line = false; 
    } else {
      record_vector.push_back(fields);
    }
	}
	return record_vector;
}

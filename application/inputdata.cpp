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

		while (!tokenizer.at_end()) { // push tokens from each line into vector
			string field = tokenizer.next_token();
			fields.push_back(field);
		}
    if (first_line) {
      first_line = false; // skip the first line
    } else {
      record_vector.push_back(fields);
    }
	}
	return record_vector;
}

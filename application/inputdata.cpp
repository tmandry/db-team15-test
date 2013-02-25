#include "inputdata.h"

using namespace std; 

vector<Record> read_data(string filename)
{
	ifstream data_file;
	string line;
	data_file.open(filename);

	// look through each line
	while(getline(data_file, line)) {
		vector<string> values;
		StringTokenizer tokenized_line(line); 
		tokenized_line.set_delim(',');
		while(!tokenized_line.at_end()) { // push tokens from each line into vector
			string entry = tokenized_line.next_token();
			values.push_back(entry);
		}
		
	}
	return vector<Record>();
}
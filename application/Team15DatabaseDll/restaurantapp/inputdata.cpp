#include "inputdata.h"

using namespace std; 

Record read_data(string filename)
{
	ifstream data_file;
	string line;
	data_file.open(filename);
	vector<string> column_labels;
	vector<string> values;
	int column_numbers;

	// look through each line
	while(getline(data_file, line)) {
		StringTokenizer tokenized_line(line); 
		tokenized_line.set_delim(',');
		while(!tokenized_line.at_end()) { // push tokens from each line into vector
			string entry = tokenized_line.next_token();
			values.push_back(entry);
		}
		column_numbers = values.size();
	}
	// push labels into separate vector
	for(int i = 0; i < column_numbers; i ++)
		column_labels[i] = values[i];
	return Record(values); // how to include Record...?
}
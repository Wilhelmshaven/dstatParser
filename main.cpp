//==============================
//   By Lihongjie
//  lihongjie@cmss.chinamobile.com
//==============================

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char **argv)
{
	if (argc != 6)
		return 0;

	string inputFile = argv[1];
	string outputFile = argv[2];
	double epoch_begin = stod(argv[3]);
	double epoch_end = stod(argv[4]);
	int column = stoi(argv[5]);

	if (column < 1 || epoch_end < epoch_begin)
	{
		cout << "Invalid input." << endl;
		return 0;
	}

	// For test
	//string inputFile = "test.csv";
	//string outputFile = "Summary.csv";
	//long epoch_begin = 1486452920;
	//long epoch_end = 1486452930;
	//int column = 40;
	// Test end.

	ifstream fileStream;
	fileStream.open(inputFile);

	if (!fileStream.is_open())
	{
		cout << "Can't open input file" << endl;
		return 0;
	}

	ofstream output;
	output.open(outputFile, ios::app);

	string buf;
	vector<double> sum(column, 0);
	int pivot;
	const char delim = ',';

	// We don't need the line 1-7
	for (int i = 0; i < 7; ++i)
	{
		getline(fileStream, buf);
	}

	char *strTmp;

	while (!fileStream.eof())
	{
		getline(fileStream, buf);
		if (buf.empty())
			break;

		pivot = 2;
		
		char* token = strtok_s(&buf[0], &delim, &strTmp);
		long epoch = stol(token);

		// We don't need the row 1-2
		token = strtok_s(NULL, &delim, &strTmp);
		token = strtok_s(NULL, &delim, &strTmp);

		// Select proper row & sum
		if (epoch >= epoch_begin && epoch <= epoch_end)
		{	
			while (token)
			{			
				sum[pivot] += stod(token);
				++pivot;
				token = strtok_s(NULL, &delim, &strTmp);
			}
		}
	}

	// Get average
	long count = epoch_end - epoch_begin + 1;
	for (int i = 0; i < column; ++i)
	{
		sum[i] /= count;
	}

	// Write results to file
	output << inputFile << ",";
	if (column > 0)
		output << sum[0];
	for (int i = 1; i < column; ++i)
	{
		output << "," << sum[i];
	}

	return 0;
}
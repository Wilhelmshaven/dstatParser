//==============================
//
//  By Lihongjie 2017-02-27
//  lihongjie@cmss.chinamobile.com
//
//  For Linux: 
//    g++ dstatParser.cpp -o dstatParser -std=c++11
//    dstatParser <input> <output> <epoch_start> <epoch_finish> <column_in_csv>
//
//  For Windows: 
//    You may need C++ Distribution 2013
//    Using bat -> .\dstatParser.exe <input> <output> <epoch_start> <epoch_finish> <column_in_csv>
//
//==============================

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char **argv)
{
	if (argc != 6)
	{
		cout << "Invalid input"<<endl;
		return 0;
	}

	string inputFile = argv[1];
	string outputFile = argv[2];
	double epoch_begin = stod(argv[3]);
	double epoch_end = stod(argv[4]);
	int column = stoi(argv[5]) - 2;

	if (column < 1 || epoch_end < epoch_begin)
	{
		cout << "Invalid input." << endl;
		return 0;
	}

	////For test
	//string inputFile = "ds-Ttnf-YFQ-ONEST-8-31.csv";
	//string outputFile = "Summary.csv";
	//long epoch_begin = 1487957600;
	//long epoch_end = 1487971610;
	//int column = 12 - 2;
	////Test end.

	ifstream inputStream;
	inputStream.open(inputFile);
	
	ofstream outputStream;
	outputStream.open(outputFile, ios_base::app);

	if (!inputStream.is_open() || !outputStream.is_open())
	{
		cout << "Can't open file" << endl;
		return 0;
	}

	string buf;
	vector<double> sum(column, 0);
	int pivot = 0;
	long epoch = 0;
	const char delim = ',';
	
	// We don't need the row 1-7
	for (int i = 0; i < 7; ++i)
	{
		getline(inputStream, buf);
	}

	while (!inputStream.eof())
	{
		getline(inputStream, buf, delim);
		if (buf.empty())
			break;

		epoch = stol(buf);

		if (epoch >= epoch_begin && epoch <= epoch_end)
		{
			// We don't need col 2
			getline(inputStream, buf, delim);
			
			for (int i = 0; i < column - 1; ++i)
			{
				getline(inputStream, buf, delim);
				sum[i] += stod(buf);
			}
			getline(inputStream, buf);
			sum[column - 1] += stod(buf);
		}
		else
		{
			// Abort this line
			getline(inputStream, buf);
		}
		
	}

	// Get average
	long count = epoch_end - epoch_begin + 1;
	for (int i = 0; i < column; ++i)
	{
		sum[i] /= count;
	}

	// Write results to file
	outputStream << inputFile << ",";
	if (column > 0)
		outputStream << sum[0];
	for (int i = 1; i < column; ++i)
	{
		outputStream << "," << sum[i];
	}

	outputStream << endl;

	inputStream.close();
	outputStream.close();

	return 0;
}
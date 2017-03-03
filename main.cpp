//==============================
//
//  By Lihongjie 2017-02-27
//  lihongjie@cmss.chinamobile.com
//
//  For Linux: 
//    g++ dstatParser.cpp -o dstatParser -std=c++11
//    ./dstatParser <input> <output> <epoch_start> <epoch_finish>
//
//  For Windows: 
//    You may need C++ Distribution 2013
//    Using bat -> .\dstatParser.exe <input> <output> <epoch_start> <epoch_finish>
//
//  Eg. .\dstatParser.exe ds-Ttnf-YFQ-ONEST-8-{31,32,46,47}.csv Summary.csv 1487957600 1487971610
//
//  Notes:
//    03-03: remove column param & handle simple batch input by '{num1,num2,num3}'
//
//==============================

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char **argv)
{
	if (argc != 5)
	{
		cout << "Invalid input" << endl;
		return 0;
	}

	string inputFile = argv[1];
	string outputFile = argv[2];
	long epoch_begin = stol(argv[3]);
	long epoch_end = stol(argv[4]);

	if (epoch_end < epoch_begin)
	{
		cout << "Invalid epoch." << endl;
		return 0;
	}

	////For test
	//string inputFile = "ds-Ttnf-YFQ-ONEST-8-{31,32,46,47}.csv";
	//string outputFile = "Summary.csv";
	//long epoch_begin = 1487957600;
	//long epoch_end = 1487971610;
	////Test end.

	vector<string> inputFileList;
	if (inputFile.find('{') == string::npos)
	{
		inputFileList.push_back(inputFile);
	}
	else
	{
		int pivot_begin = inputFile.find('{');
		int pivot_end = inputFile.find('}');

		string headStr = inputFile.substr(0, pivot_begin);
		string tailStr = inputFile.substr(pivot_end + 1, inputFile.length() - pivot_end);
		string midStr = inputFile.substr(pivot_begin + 1, pivot_end - pivot_begin - 1);

		while (midStr.find(',') != string::npos)
		{
			inputFileList.push_back(headStr + midStr.substr(0, midStr.find(',')) + tailStr);
			midStr = midStr.substr(midStr.find(',') + 1, midStr.length() - midStr.find(','));
		}
		inputFileList.push_back(headStr + midStr + tailStr);
	}

	for (int fileCount = 0; fileCount < inputFileList.size(); ++fileCount)
	{
		ifstream inputStream;
		inputStream.open(inputFileList[fileCount]);

		ofstream outputStream;
		outputStream.open(outputFile, ios_base::app);

		if (!inputStream.is_open() || !outputStream.is_open())
		{
			cout << "Can't open file" << endl;
			break;
		}

		string buf;
		int pivot = 0;
		long epoch = 0;
		const char delim = ',';

		// We don't need the row 1-7, line 7 is used for counting commas
		for (int i = 0; i < 7; ++i)
		{
			getline(inputStream, buf);
		}

		int column = 0;
		for (int i = 0; i < buf.length(); ++i)
		{
			if (buf[i] == ',')
				++column;
		}
		if (column < 2)
		{
			// No data?
			cout << "No data in CSV." << endl;
			break;
		}

		--column;
		vector<double> sum(column, 0);

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
		outputStream << inputFileList[fileCount] << ",";
		if (column > 0)
			outputStream << sum[0];
		for (int i = 1; i < column; ++i)
		{
			outputStream << "," << sum[i];
		}

		outputStream << endl;

		inputStream.close();
		outputStream.close();
	}

	return 0;
}
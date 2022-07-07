#include <iostream>
#include <fstream>
#include <pthread.h>
#include <string>
#include <vector>
#include "DataBase.h"

using namespace std;

bool DataBase::makeFile(string file_name, fstream& fstream_obj)
{
	fstream_obj.open(file_name, ios::in | ios::out);
	return fstream_obj.is_open();
}

bool DataBase::makeAppendFile(string file_name, fstream& fstream_obj)
{
	fstream_obj.open(file_name, ios::app);
	return fstream_obj.is_open();
}

void DataBase::readData(string fileName, vector<string>& buffer_vec, int startLine, int endLine)
{
	fstream fileObj;
	string buffer;
	DataBase::makeFile(fileName, fileObj);
	int tmp_idx = 0;
	while (fileObj.peek() != EOF)
	{
		getline(fileObj, buffer);
		if (tmp_idx >= startLine && (tmp_idx <= endLine || endLine == -1))
			buffer_vec.push_back(buffer);
		else if (tmp_idx >= startLine)
			break;
		tmp_idx++;
	}
	fileObj.close();
}

void DataBase::writeData(string fileName, string writeData, bool appendBool)
{
	fstream fileObj;
	if (appendBool)
		DataBase::makeAppendFile(fileName, fileObj);
	else
	{
		ofstream tmp_ofstream;
		tmp_ofstream.open(fileName);
		tmp_ofstream.close();
		DataBase::makeAppendFile(fileName, fileObj);
	}
	fileObj.seekp(0, ios::end);
	fileObj.write(writeData.c_str(), writeData.size());
	fileObj.close();
}

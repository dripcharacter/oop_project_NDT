#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <pthread.h>
#include <fstream>

using namespace std;

class DataBase
{
public:
	static bool makeFile(string file_name, fstream& fstream_obj);
	static bool makeAppendFile(string file_name, fstream& fstream_obj);
	static void readData(string fileName, vector<string>& buffer_vec, int startLine, int endLine);
	static void writeData(string fileName, string writeData, bool appendBool);
};

#endif

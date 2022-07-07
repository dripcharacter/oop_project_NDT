#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include "DataBase.h"
#include "user.h"
using namespace std;

void User::doLogout() {
    ofstream fout;
    fout.close(); // 읽어오던 파일 닫음
}

void NormalUser::updateDataInDB(string fileName, string updateData, int updateMode) {
    //updateMode==0이면 pw수정, ==1이면 수강신청리스트 수정
    if (updateMode == 1)
        DataBase::writeData(fileName, updateData + "\n", true);
    else if (updateMode == 0)
    {
        vector<string> fileLines_vec;
        vector<string> id_vec;
        stringstream tmp_str_stream;
        DataBase::readData(fileName, fileLines_vec, 0, -1);
        int tmp_idx;
        for (tmp_idx = 0; tmp_idx < fileLines_vec.size(); tmp_idx++)
        {
            string tmp_str;
            tmp_str_stream.str(fileLines_vec[tmp_idx]);
            getline(tmp_str_stream, tmp_str, ' ');
            id_vec.push_back(tmp_str);
        }

        int id_idx = -1;
        tmp_idx = 0;
        while (true)
        {
            if (this->getID() == id_vec[tmp_idx])
            {
                id_idx = tmp_idx;
                break;
            }
            tmp_idx++;
        }
        string tmp_str;
        string result_str = id_vec[id_idx] + " " + updateData;
        tmp_str_stream.str(fileLines_vec[id_idx]);
        getline(tmp_str_stream, tmp_str, ' ');
        getline(tmp_str_stream, tmp_str, ' ');
        getline(tmp_str_stream, tmp_str, ' ');
        result_str += " " + tmp_str;
        getline(tmp_str_stream, tmp_str, ' ');
        result_str += " " + tmp_str;
        fileLines_vec[id_idx] = result_str;

        tmp_idx = 0;
        DataBase::writeData(fileName, fileLines_vec[tmp_idx] + "\n", false);
        for (tmp_idx = 1; tmp_idx < fileLines_vec.size(); tmp_idx++)
            DataBase::writeData(fileName, fileLines_vec[tmp_idx] + "\n", true);
    }
}
NormalUser::NormalUser(string id, string pw, string name, int  studentNumber) {
    this->studentNumber = studentNumber;
    this->id = id;
    this->password = pw;
    this->name = name;
}
string NormalUser::getID() {
    return this->id;
}
string NormalUser::getPw() {
    return this->password;
}
string NormalUser::getName() {
    return this->name;
}
int NormalUser::getStudentNumber() {
    return this->studentNumber;
}
bool NormalUser::changePw(string pw) {
    if (pw == "") {
        return false;
    }
    this->password = pw;
    NormalUser::updateDataInDB("studentData.txt", pw, 0);
    return true;
}

void NormalUser::getEnrollmentLog() {
    //수강신청 database 완성되야 가능
    vector<string> enrollmentLogList;
    string fileName = this->getName() + "_cur_state.txt";
    int startLine = 0;
    int endLine = -1;

    DataBase::readData(fileName, enrollmentLogList, startLine, endLine);

    int tmp_idx;
    for (tmp_idx = 0; tmp_idx < enrollmentLogList.size(); tmp_idx++)
        cout << enrollmentLogList[tmp_idx] << endl;
}

void getCurRemain()
{
    vector<string> course_vec;
    DataBase::readData("courseData.txt", course_vec, 0, -1);

    int tmp_idx=0;
    for(tmp_idx=0; tmp_idx<course_vec.size(); tmp_idx++)
        cout<<course_vec[tmp_idx]<<endl;
}

AdminUser::AdminUser(string id, string pw, string name) {
    this->id = id;
    this->password = pw;
    this->name = name;
}
string AdminUser::getID() {
    return this->id;
}
string AdminUser::getPw() {
    return this->password;
}
string AdminUser::getName() {
    return this->name;
}
//추가 list의 몇 번째 entry에 대해서 판단하는지
void AdminUser::addStudent(int entryLine, bool addBool) {
	vector <string> addWait_vec;
    DataBase::readData("addWaiting.txt", addWait_vec, 0, -1);
    if (addBool)
    {
        string tmp_str = addWait_vec[entryLine];
        DataBase::writeData("studentData.txt", tmp_str + "\n", true);
    }
    addWait_vec.erase(addWait_vec.begin() + (entryLine));
    if(addWait_vec.size()!=0)
	{
		int tmp_idx = 0;
    	DataBase::writeData("addWaiting.txt", addWait_vec[tmp_idx] + "\n", false);
    	for (tmp_idx = 1; tmp_idx < addWait_vec.size(); tmp_idx++)
        	DataBase::writeData("addWaiting.txt", addWait_vec[tmp_idx] + "\n", true);
	}
	else
		DataBase::writeData("addWaiting.txt", "", false);
}
void AdminUser::updateCourse(string className, string time, string professorName, int fullNumber, bool delBool) {
    //수강신청 database 완성되야 가능
    string course_str = className + " " + time + " " + professorName + " " + to_string(fullNumber);
    vector<string> course_vec;
    stringstream tmp_str_stream;
    vector<string> className_vec;
    DataBase::readData("courseData.txt", course_vec, 0, -1);

    int tmp_idx;
    for (tmp_idx = 0; tmp_idx < course_vec.size(); tmp_idx++)
    {
        string tmp_str;
        tmp_str_stream.str(course_vec[tmp_idx]);
        getline(tmp_str_stream, tmp_str, ' ');
        className_vec.push_back(tmp_str);
    }

    int id_idx = -1;
    tmp_idx = 0;
    while (true)
    {
        if (className == className_vec[tmp_idx])
        {
            id_idx = tmp_idx;
            break;
        }
        tmp_idx++;
    }

    string tmp_str;
    tmp_str_stream.str(course_vec[id_idx]);
    getline(tmp_str_stream, tmp_str, ' ');
    getline(tmp_str_stream, tmp_str, ' ');
    getline(tmp_str_stream, tmp_str, ' ');
    getline(tmp_str_stream, tmp_str, ' ');
    getline(tmp_str_stream, tmp_str, ' ');
    course_str=course_str+" "+tmp_str;

    if (delBool)
        course_vec.erase(course_vec.begin() + id_idx);
    else
        course_vec[id_idx] = course_str;
    if(course_vec.size()!=0)
	{
		tmp_idx = 0;
    	DataBase::writeData("courseData.txt", course_vec[tmp_idx] + "\n", false);
    	for (tmp_idx = 1; tmp_idx < course_vec.size(); tmp_idx++)
        	DataBase::writeData("courseData.txt", course_vec[tmp_idx] + "\n", true);
	}
	else
		DataBase::writeData("courseData.txt", "", false);
}
void AdminUser::addCourse(string className, string time, string professorName, int fullNumber) {
    //수강신청 database 완성되야 가능
    string course_str = className + " " + time + " " + professorName + " " + to_string(fullNumber) + " " + to_string(fullNumber);
    DataBase::writeData("courseData.txt", course_str + "\n", true);
}

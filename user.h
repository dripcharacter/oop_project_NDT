#ifndef _USER_H_
#define _USER_H_
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "DataBase.h"
using namespace std;


class User {
protected:
    string password;
    string id;
    string name;
    //int loginFailedCnt;
    void doLogout(void);
public:
    virtual string getName(void) = 0;
    virtual string getID(void) = 0;
    virtual string getPw(void) = 0;
};


class NormalUser : public User {
private:
    int studentNumber;
    void updateDataInDB(string, string, int);
public:
    NormalUser(string, string, string, int);
    string getID();
    string getPw();
    string getName();
    int getStudentNumber();
    bool changePw(string);
    void getEnrollmentLog();
    void getCurRemain();
};

class AdminUser : public User {
public:
    AdminUser(string, string, string);
    string getID();
    string getPw();
    string getName();
    void addStudent(int, bool);
    void updateCourse(string, string, string, int, bool);
    void addCourse(string, string, string, int);
};
#endif
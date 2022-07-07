#ifndef _MENU_H_
#define _MENU_H_
#include <iostream>
#include "simulation.h"
#include "user.h"
#include "DataBase.h"
#include "menu.h"
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

class Menu {
public:
	void getUserData();
	void getAdminData();
	void getAddWaitingData();
	void getAllCourseData();
	virtual int go(int number) = 0;
};

class MainMenu : public Menu {
public:
	int go(int number);
	void simulation();
	bool login(string id = "", string pw = "");
	bool signUp(string id = "", string pw = "", string name = "", int student_number = 0);
	bool adminLogin(string id = "", string pw = "");
};

class NormalUserMenu : public Menu {
public:
	int go(int number);
};

class AdminUserMenu : public Menu {
public:
	int go(int number);
};

#endif

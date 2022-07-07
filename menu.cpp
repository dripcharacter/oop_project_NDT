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
int loginuser;
std::fstream studentData("studentData.txt");
std::fstream waitingData("addWaiting.txt");
std::fstream allCourse("courseData.txt");
vector<NormalUser> students;
vector<string> addWaiting_vec;
vector<string> courseData_vec;
std::fstream adminData("adminData.txt");
vector<AdminUser> admins;

void Menu::getAddWaitingData() {
    if(waitingData.is_open())
    {
        waitingData.close();
        waitingData.open("addWaiting.txt", ios::in|ios::out);
    }
    addWaiting_vec.clear();
    DataBase::readData("addWaiting.txt", addWaiting_vec, 0, -1);
}

void Menu::getAllCourseData() {
    if(allCourse.is_open())
    {
        allCourse.close();
        allCourse.open("courseData.txt", ios::in|ios::out);
    }
    courseData_vec.clear();
    DataBase::readData("courseData.txt", courseData_vec, 0, -1);
}

void Menu::getUserData() {

	if(studentData.is_open())
    {
        studentData.close();
        studentData.open("studentData.txt", ios::out|ios::in);
    }
	students.clear();
    string id;
	string pw;
	string name;
	string studentNumber;
	while (studentData.eof() == 0) {
		string tmp_str;
		stringstream ss;
		getline(studentData, tmp_str);
		if(tmp_str.length()==0)
			break;

		ss.str(tmp_str);
		getline(ss, id, ' ');
		getline(ss, pw, ' ');
		getline(ss, name, ' ');
		getline(ss, studentNumber, ' ');

		NormalUser student(id, pw, name, stoi(studentNumber));
		students.push_back(student); //백터에 텍스트 student 정보 저장
	}
}

void Menu::getAdminData() {

    if(adminData.is_open())
    {
        adminData.close();
        adminData.open("adminData.txt", ios::in|ios::out);
    }
    admins.clear();
	string id;
	string pw;
	string name;
	while (adminData.eof() == 0) {
		adminData >> id;
		adminData >> pw;
		adminData >> name;

		AdminUser admin(id, pw, name);
		admins.push_back(admin); //백터에 텍스트 admin정보 저장
	}
}

int MainMenu::go(int number) {
	if (number == 0) {
		cout << endl << endl;
		cout << "1.Login " << endl;
		cout << "2.Sign up" << endl;
		cout << "3.simulation" << endl;
		cout << "4.Admin login" << endl;
		cout << "5.Exit" << endl;
		cout << "-----------------------------------------------------------------" << endl;
	}

	else if (number == 1) {
		string id = "";
		string pw = "";
		bool check = 1; //계속 실행
		int try_number = 0; //아이디 입력 회수
		int try_number2 = 0; //비밀번호 입력 회수
		cout << endl << endl << "login page" << endl;

		while (check) {
			try_number += 1;
			cout << "ID : ";
			cin >> id;
			if (login(id) == 1) {
				break;//id 확인 끝
			}

			if (try_number == 3) {
				cout << "------------------------------------" << endl;
				cout << "** three time wrong id **" << endl;
				cout << "------------------------------------";
				cout << endl << endl << endl;
				return 0; //다시 메인 메뉴로 돌아감
			}
		}
		while (login(id) == 1) {
			try_number2 += 1;
			cout << "pw : ";
			cin >> pw;
			if (login(id, pw) == 1) {
				cout << endl << "** login success **" << endl << endl;
				return 1; //아이디 비밀번호 전부 맞췄으므로 1의 메뉴로 이동 => user 정보 어떻게 가져와야 할지 정해야 함
			}


			if (try_number2 == 3) {
				cout << "** three time wrong pw **" << endl;
				return 0;  //다시 메인메뉴로 돌아감
			}
		}

	}

	else if (number == 2) {
		string id = "";
		string pw = "";
		string name = "";
		bool check = 1;
		int student_number = 0;
		cout << endl << endl << "signUp page" << endl;
		while (check) {
			cout << "ID: ";
			cin >> id;
			if (signUp(id) == 1) {
				break;
			}
		}
		while (check) {
			cout << "pw: ";
			cin >> pw;
			if (signUp(id, pw) == 1) {
				break;
			}
		}
		while (check) {
			cout << "name: ";
			cin >> name;
			if (signUp(id, pw, name) == 1) {
				break;
			}
		}
		while (check) {
			cout << "Student number: ";
			cin >> student_number;
			if (signUp(id, pw, name, student_number) == 1) {
				cout << endl << "** sign up success **" << endl;
				break;
			}
		}
		return 0;
	}

	else if (number == 3) {
		simulation();
		return 0;
	}

	else if (number == 4) {
		string id = "";
		string pw = "";
		bool check = 1; //계속 실행
		int try_number = 0; //아이디 입력 회수
		int try_number2 = 0; //비밀번호 입력 회수
		cout << endl << endl << "Admin login page" << endl;

		while (check) {
			try_number += 1;
			cout << "ID : ";
			cin >> id;
			if (adminLogin(id) == 1) {
				break;//id 확인 끝
			}

			if (try_number == 3) {
				cout << "------------------------------------" << endl;
				cout << "** three time wrong id **" << endl;
				cout << "------------------------------------";
				cout << endl << endl << endl;
				return 0; //다시 메인 메뉴로 돌아감
			}
		}
		while (adminLogin(id) == 1) {
			try_number2 += 1;
			cout << "pw : ";
			cin >> pw;
			if (adminLogin(id, pw) == 1) {
				cout << endl << "** login success **" << endl << endl;
				return 2; //아이디 비밀번호 전부 맞췄으므로 1의 메뉴로 이동 => user 정보 어떻게 가져와야 할지 정해야 함
			}


			if (try_number2 == 3) {
				cout << "** three time wrong pw **" << endl;
				return 0;  //다시 메인메뉴로 돌아감
			}
		}
	}




	else if (number == 5) {

		cout << endl << "-----------------------------------------------------------------" << endl;
		cout << "           The application for admission is closed";
		cout << endl << "-----------------------------------------------------------------" << endl;
		return 10;
	}

	else {
		cout << endl << "-----------------------------------------------------------------" << endl;
		cout << "           **  error: no such a menu  **" << endl;
		cout << "-----------------------------------------------------------------" << endl << endl;
		return 0;
	}
}

void MainMenu::simulation() {
    getUserData();
	DoSimulation(students);
}

bool MainMenu::login(string id, string pw) {
	getUserData();

	int tmp_idx=0;
	bool id_bool=false;
	for(tmp_idx=0; tmp_idx<students.size(); tmp_idx++)
    {
	    if(id==students[tmp_idx].getID())
        {
	        id_bool=true;
            loginuser=tmp_idx;
            break;
        }
    }

	if (id_bool && (pw == "")) {
		return 1;
	}
	if (id_bool) {
		if (students[loginuser].getPw() == pw) {
			return 1;
		}
		else {
			return 0;
		}
	}
	return 0;
}

bool MainMenu::signUp(string id, string pw, string name, int student_number) {
	//텍스트 파일에서 데이터 불러와야 함
	getUserData();

    int tmp_idx=0;
    bool id_bool=false;
    bool stdNum_bool=false;
    for(tmp_idx=0; tmp_idx<students.size(); tmp_idx++)
    {
        if(id==students[tmp_idx].getID())
        {
            id_bool=true;
            break;
        }
    }
    for(tmp_idx=0; tmp_idx<students.size(); tmp_idx++)
    {
        if(student_number==students[tmp_idx].getStudentNumber())
        {
            stdNum_bool=true;
            break;
        }
    }

	if (id_bool/* && student_number != 21512452334/*마스터코드*/) {
		return 0;  //아이디가 겹침
	}
	else {
		if (stdNum_bool) {
			return 0; //학번이 겹침
		}
		else {
            if(id!=""&&pw!=""&&name!=""&&student_number!=0)
			{
				string tmp_str=id+" "+pw+" "+name+" "+ to_string(student_number)+"\n";
				DataBase::writeData("addWaiting.txt", tmp_str, true);
			}
			return 1;
		}
	}

	return 0;
}

bool MainMenu::adminLogin(string id, string pw) {
	getAdminData();

    int tmp_idx=0;
    bool id_bool=false;
    for(tmp_idx=0; tmp_idx<admins.size(); tmp_idx++)
    {
        if(id==admins[tmp_idx].getID())
        {
            id_bool=true;
            loginuser=tmp_idx;
            break;
        }
    }

	if (id_bool && (pw == "")) {
		return 1;
	}
	if (id_bool) {
		if (admins[loginuser].getPw() == pw) {
			return 1;
		}
		else {
			return 0;
		}
	}
	return 0;
}

int NormalUserMenu::go(int number) {
	if (number == 0) {
		cout << "1.Registration " << endl;//done
		cout << "2.My registration log" << endl;   //maybe
		cout << "3.Open course & capacity" << endl;  //done
		cout << "4.Change password" << endl;  //done
		cout << "5.Logout" << endl;  //done
		cout << "-----------------------------------------------------------------" << endl;
	}
	else if (number == 1) {
	    getAllCourseData();
		string className;
		int i = 1;
		string finalData;
		bool check;
		int tmp_idx=0;
		for(tmp_idx=0; tmp_idx<courseData_vec.size(); tmp_idx++)
		    cout<<courseData_vec[tmp_idx]<<endl;
		cout << "input 0 to delete input 1 to add : ";
		cin >> check;
		cout << "input class name : ";
		cin >> className;
		if (check == 0) {
			finalData = "del " + className + " " + students[loginuser].getID();
		}
		else {
			finalData = "add " + className + " " + students[loginuser].getID();
		}
        DataBase::writeData(students[loginuser].getID()+"_registration_list.txt", finalData+"\n", true);
		return 1;
	}
	else if (number == 2) {
		cout << "------- your EnrollmentLog --------" << endl << endl;
		students[loginuser].getEnrollmentLog();
		cout << endl << endl << "         ** return to menu **        "<<endl;
		return 1;
	}
	else if (number == 3) {
        getAllCourseData();
		bool check = 0;
		int tmp_idx=0;
        for(tmp_idx=0; tmp_idx<courseData_vec.size(); tmp_idx++)
            cout<<courseData_vec[tmp_idx]<<endl;

		cout << "**input 0 to return to menu**" << endl; //사실은 1 안누르고 엔터쳐도 메뉴로 돌아감
		cin >> check;
		if (check == 0) {
			return 1;
		}

		return 1;
	}
	else if (number == 4) {
		getUserData();
		string pw;
		cout << "password: ";
		cin >> pw;
		students[loginuser].changePw(pw);
		return 1;
	}
	else if (number == 5) {
		loginuser = 9999999;
		cout << endl << "** logout success **" << endl;
		return 0;
	}
	else {
		cout << "error: no such a menu" << endl << endl;
		return 1;
	}
}
	
int AdminUserMenu::go(int number) {
	if (number == 0) {
		cout << "1.Signup permit " << endl;  //done
		cout << "2.Make new course" << endl;  //done
		cout << "3.Delete & change course" << endl;
		cout << "4.logout" << endl;  //done
		cout << "-----------------------------------------------------------------" << endl << endl;
	}
	else if (number == 1) {
	    getAddWaitingData();
	    int tmp_idx=0;
	    for(tmp_idx=0; tmp_idx<addWaiting_vec.size(); tmp_idx++)
        {
	        cout<<addWaiting_vec[tmp_idx]<<endl;
        }
		int line = 0;
		bool check = 0;
		cout << endl << "**Enter 0 to process all at once** " << endl;
		cout << "choose number to process: ";
		cin >> line;
		cout << "press 1 to accept, press 0 to reject" << endl;
		cout << " : ";
		cin >> check;
		if (line == 0) {
			for (tmp_idx=0; tmp_idx<addWaiting_vec.size(); tmp_idx++) {
				admins[loginuser].addStudent(tmp_idx, check);
			}
		}
		else if (addWaiting_vec.size() < line) {
			return 2;
		}
		else {
			admins[loginuser].addStudent(line-1, check);
		}
		return 2;
	}
	else if (number == 2) {
		string className;
		string time;
		string professorName;
		int fullNumber = 0;
		cout << "class name: ";
		cin >> className;
		cout << "class time: ";
		cin >> time;
		cout << "professor name: ";
		cin >> professorName;
		cout << "fullNumber: ";
		cin >> fullNumber;
		admins[loginuser].addCourse(className, time, professorName, fullNumber);
		return 2;
	}
	else if (number == 3) {
	    getAllCourseData();

		int line = 0;
		bool check = 0;
		
		string className;
		string time;
		string professorName;
		int fullNumber;

		int tmp_idx=0;
		for(tmp_idx=0; tmp_idx<courseData_vec.size(); tmp_idx++)
        {
		    cout<<courseData_vec[tmp_idx]<<endl;
        }

		//어떤 과목을 삭제하거나, 수정할지 선택하고 수정 할 정보 이력하는 코드 짜야 함
		cout << "**input 0 to change, input 1 to delete**" << endl;
		cin >> check;
		cout << "class name: ";
		cin >> className;
		cout << "time: ";
		cin >> time;
		cout << "professor: ";
		cin >> professorName;
		cout << "full number: ";
		cin >> fullNumber;
		admins[loginuser].updateCourse(className, time, professorName, fullNumber, check);
		getAllCourseData();


		return 2;
	}
	else if (number == 4) {
		loginuser = 9999999;
		cout << endl << "** logout success **" << endl;
		return 0;
	}
	else {
		cout << "error: no such a menu" << endl << endl;
		return 2;
	}
}

int main() {
	Menu *main = new MainMenu();
	Menu* normal_user = new NormalUserMenu();
	Menu* admin_user = new AdminUserMenu();
	int menu=0;
	int choose_number;
	while(main) {
		switch (menu) {
		case 0:
			main->go(0);
			cout << "chose number : ";
			cin >> choose_number;
			menu = main->go(choose_number);
			break;
		case 1:
			normal_user->go(0);
			cout << "chose number : ";
			cin >> choose_number;
			menu = normal_user->go(choose_number);
			break;
		case 2:
			admin_user->go(0);
			cout << "chose number : ";
			cin >> choose_number;
			menu = admin_user->go(choose_number);
			break;
		case 10:
			break;
		}
		if (menu == 10) {
			break;
		}
	}
}

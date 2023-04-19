# oop_project_NDT
## 1. Problem Situation
### At Second Semester of 2022, there was problem at course registration. If someone tries to register at Automata course, then that person had to wait forever unless he or she refresh the webpage. I thought that this problem was occured by deadlock at registration DB. So, I tried to make multi-user course registration simulator with mutex lock to prevent deadlock.
## 2. Sturcture of project
### This project is totally based on Command Line Interface from C++ and .txt file to use as DataBase.(Because of the restriction of oop course. This project was from oop class.)
### 2.1. Menu.cpp
#### Menu.cpp controlls every functionallity from other source file(DataBase.cpp user.cpp simulation.cpp)
### 2.2. user.cpp
#### user.cpp is a functionallity for users. Users are divided into two categories. Normal users and Admin users.
### 2.3. DataBase.cpp
#### DataBase.cpp is a functionallity for read or write to files. These files will behave like database, so the name of file IO is DataBase.cpp.
### 2.4. simulation.cpp
#### simulation.cpp is a core part of this multi-user course registration simulator. It uses mutex to prevent deadlock.
## 3. how to build project
```
make menu
```
But Makefile don't have cleanup functionallity. I'll add it later.

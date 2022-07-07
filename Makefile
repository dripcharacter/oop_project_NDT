DataBase.o : DataBase.h DataBase.cpp
	g++ -c DataBase.cpp -lpthread

simulation.o : simulation.h user.h DataBase.h simulation.cpp
	g++ -c simulation.cpp -lpthread

user.o : DataBase.h user.h user.cpp
	g++ -c user.cpp -lpthread

menu.o : user.h DataBase.h simulation.h menu.h menu.cpp
	g++ -c menu.cpp -lpthread

menu : DataBase.o simulation.o user.o menu.o
	g++ DataBase.o simulation.o user.o menu.o -o menu -lpthread

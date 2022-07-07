#ifndef PRIVATE_SIMULATION_H
#define PRIVATE_SIMULATION_H
#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <vector>
#include <sstream>
#include <unistd.h>
#include <pthread.h>
#include "user.h"
using namespace std;

void DoSimulation(vector<NormalUser>& normalUser_vec);
void* MutexInsert(void* args_struct_ptr);

#endif //PRIVATE_SIMULATION_H

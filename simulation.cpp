#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <vector>
#include <unistd.h>
#include <pthread.h>
#include <sstream>
#include "user.h"
#include "DataBase.h"
#include "simulation.h"
#define SLEEP_CONST 1

using namespace std;

struct course
{
    string className;
    string time;
    string professorName;
    int fullNumber;
    int cur_remain_seat;
};

struct thread_args
{
    string thread_id;
    int* cur_pthread_cnt;
    pthread_mutex_t* fileMutex;
    queue<string>* thread_queue;
    vector<course>* course_cur_info_vec_ptr;
};

void DoSimulation(vector<NormalUser>& normalUser_vec);
void* MutexInsert(void* args_struct_ptr);


void DoSimulation(vector<NormalUser>& normalUser_vec)
{
    static pthread_mutex_t fileMutex;
    static int cur_pthread_cnt = 0;
    static queue<string> thread_queue;
    static vector<course> course_cur_info_vec;
    vector<string> course_string_vec;
    vector<thread_args> thread_args_vec;
    int tmp_idx;
    pthread_t tid_arr[normalUser_vec.size()];

    DataBase::readData("courseData.txt", course_string_vec, 0, -1);

    for(tmp_idx=0; tmp_idx<course_string_vec.size(); tmp_idx++)
    {
        stringstream ss;
        string tmp_class;
        string tmp_time;
        string tmp_prof;
        string tmp_full;
        string tmp_cur_seat;

        ss.str(course_string_vec[tmp_idx]);
        getline(ss, tmp_class, ' ');
        getline(ss, tmp_time, ' ');
        getline(ss, tmp_prof, ' ');
        getline(ss, tmp_full, ' ');
        getline(ss, tmp_cur_seat, ' ');

        course tmp_course={tmp_class, tmp_time, tmp_prof, stoi(tmp_full), stoi(tmp_cur_seat)};
        course_cur_info_vec.push_back(tmp_course);
    }

    pthread_mutex_init(&fileMutex, NULL);
    for (tmp_idx = 0; tmp_idx < normalUser_vec.size(); tmp_idx++)
    {
        thread_args tmp_args = { normalUser_vec[tmp_idx].getID(), &cur_pthread_cnt, &fileMutex, &thread_queue, &course_cur_info_vec};
        thread_args_vec.push_back(tmp_args);
        cur_pthread_cnt++;
    }
    for (tmp_idx = 0; tmp_idx < normalUser_vec.size(); tmp_idx++)
    {
		pthread_create(&tid_arr[tmp_idx], NULL, MutexInsert, &thread_args_vec[tmp_idx]);
    }
    for (tmp_idx = 0; tmp_idx < normalUser_vec.size(); tmp_idx++)
    {
        pthread_join(tid_arr[tmp_idx], NULL);
    }
    pthread_mutex_destroy(&fileMutex);

    for(tmp_idx=0; tmp_idx<course_string_vec.size(); tmp_idx++)
    {
        course_string_vec[tmp_idx]=course_cur_info_vec[tmp_idx].className+" "+course_cur_info_vec[tmp_idx].time+" "+course_cur_info_vec[tmp_idx].professorName+" "+
                                                                                                                                                               to_string(course_cur_info_vec[tmp_idx].fullNumber)+" "+
                                                                                                                                                                                                                  to_string(course_cur_info_vec[tmp_idx].cur_remain_seat);
    }
    tmp_idx=0;
    DataBase::writeData("courseData.txt", course_string_vec[tmp_idx]+"\n", false);
    for (tmp_idx = 1; tmp_idx < course_string_vec.size(); tmp_idx++)
        DataBase::writeData("courseData.txt", course_string_vec[tmp_idx]+"\n", true);
}

void* MutexInsert(void* args_struct_ptr)
{
    thread_args args = *(thread_args*)args_struct_ptr;
    pthread_mutex_t fileMutex = *args.fileMutex;
    int* cur_thread_cnt_ptr = args.cur_pthread_cnt;
    queue<string> thread_queue = *args.thread_queue;
    int queue_cnt = 0;
    vector<string> registration_line_vec;
    vector<course> course_vec=*args.course_cur_info_vec_ptr;
    string buffer;
    int tmp_idx;
    stringstream tmp_str_stream;

    string file_name = args.thread_id + "_registration_list.txt";
    string cur_state_file_name = args.thread_id + "_cur_state.txt";
    string db_name = "db_req_list.txt";
    string coursefile_name="courseData.txt";
    //  registration_list.txt의 모든 것을 한줄씩 읽어와 string vector에 저장
    DataBase::readData(file_name, registration_line_vec, 0, -1);

    for (tmp_idx = 0; tmp_idx < registration_line_vec.size(); tmp_idx++)
    {
        if (queue_cnt == 0)
        {
            thread_queue.push(args.thread_id);
            queue_cnt++;
        }
        while (thread_queue.front() != args.thread_id)
            sleep(0.1);
        pthread_mutex_lock(&fileMutex);
        //db write
        DataBase::writeData(db_name, registration_line_vec[tmp_idx] + "\n", true);

        string adddel_str;
        string course_str;
        tmp_str_stream.str(registration_line_vec[tmp_idx]);
        getline(tmp_str_stream, adddel_str, ' ');//add/del
        getline(tmp_str_stream, course_str, ' ');//course_name

        int course_idx=-1;
        int course_tmp_idx=0;
        while(true)
        {
            if(course_str==course_vec[course_tmp_idx].className)//미리 idx에 따른 과목명 추출해놓고 tmp_str이랑 비교해서 course_idx정하고 이 idx에 있는 cur_remain_seat로 success/failure 정하기기
            {
                course_idx=course_tmp_idx;
                break;
            }
            course_tmp_idx++;
        }
        bool req_success_bool=true;
        if(adddel_str=="add" || adddel_str=="ADD")
        {
            if(course_vec[course_tmp_idx].cur_remain_seat<=0)
                req_success_bool=false;
            else
            {
                req_success_bool=true;
                course_vec[course_tmp_idx].cur_remain_seat--;
            }
        }
        else if(adddel_str=="del"||adddel_str=="DEL")
        {
            if(course_vec[course_tmp_idx].cur_remain_seat>=course_vec[course_tmp_idx].fullNumber)
                req_success_bool=false;
            else
            {
                req_success_bool=true;
                course_vec[course_tmp_idx].cur_remain_seat++;
            }
        }

        //string vector 현재 entry를 현재값+" done"으로 고침
        if(req_success_bool)
            DataBase::writeData(cur_state_file_name, registration_line_vec[tmp_idx] + " success\n", true);
        else
            DataBase::writeData(cur_state_file_name, registration_line_vec[tmp_idx] + " failure\n", true);
        sleep(SLEEP_CONST * (*cur_thread_cnt_ptr));
        thread_queue.pop();
        queue_cnt--;
        pthread_mutex_unlock(&fileMutex);
    }
    *cur_thread_cnt_ptr = *cur_thread_cnt_ptr - 1;
}

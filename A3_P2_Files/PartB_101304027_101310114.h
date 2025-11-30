#ifndef PARTB_101304027_101310114_H
#define PARTB_101304027_101310114_H

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <sys/sem.h>

#define NUM_RUBRIC 5
#define NUM_QUESTIONS 5
#define RUBRIC_LINE_LEN 256

struct shared_data {
    char rubric[NUM_RUBRIC][RUBRIC_LINE_LEN];  
    int current_student;                        
    int current_exam;                           
    int questions_marked[NUM_QUESTIONS];        
};

void lock(int semid);
void unlock(int semid);

void load_rubric(shared_data* var);
void load_exam(shared_data* var);
void delay(int micro_seconds);
void save_rubric(shared_data* var, int ta_ID);
void modify_rubric(shared_data* var, int ta_ID, int line, int rubric_semid);
void load_next_exam(int ta_ID, shared_data* var, int loader_semid);
void ta_process(int ta_ID, shared_data* var, int rubric_semid, int question_semid, int loader_semid);

#endif
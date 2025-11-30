#include "PartB_101304027_101310114.h"
#include <unistd.h>
#include <iostream>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string>
#include <cstdlib>

int main(int argc, char* argv[]) {

    if (argc < 2){
        std::cout << "Improper program call. Example of call: " << argv[0] << " <number_of_TAs>" << std::endl;
        return 1;
    }

    int num_TA = atoi(argv[1]);     
    if (num_TA < 2){    
        std::cout << "ERROR - Need at least two TA's" << std::endl;
        return 1;
    }

    //Creating Semaphores 
    int rubric_semid = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT);
    int question_semid = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT);
    int loader_semid = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT);

    //Initializing Semaphores
    semctl(rubric_semid, 0, SETVAL, 1);
    semctl(question_semid, 0, SETVAL, 1);
    semctl(loader_semid, 0, SETVAL, 1);

    // Creating Shared Memory 
    int shmid = shmget(IPC_PRIVATE, sizeof(shared_data), 0666 | IPC_CREAT);
    shared_data* var = (shared_data*)shmat(shmid, NULL, 0); 

    // Initializing Shared Data
    var->current_exam = 0;
    load_rubric(var);
    load_exam(var);

    if (var->current_student == 9999){
        std::cout << "The first exam loaded into memory contains student number 9999, stopping all execution" << std::endl;

        shmdt(var);
        shmctl(shmid, IPC_RMID, nullptr);
        semctl(rubric_semid, 0, IPC_RMID);
        semctl(question_semid, 0, IPC_RMID);
        semctl(loader_semid, 0, IPC_RMID);
        return 0;
    }

    // Creating TA Processes 
    for (int i = 0; i < num_TA; i++){
        pid_t pid = fork();

        if (pid < 0) {
            std::cout << "Fork Failed" << std::endl;
            return 1;
        }

        if (pid == 0) {
            ta_process(i, var, rubric_semid, question_semid, loader_semid);
            shmdt(var);
            return 0;
        }
    }

    // Waiting for all TA's to execute
    for (int i = 0; i < num_TA; i++){
        wait(NULL);
    }
    
    shmdt(var); 
    shmctl(shmid, IPC_RMID, nullptr);
    semctl(rubric_semid, 0, IPC_RMID);
    semctl(question_semid, 0, IPC_RMID);
    semctl(loader_semid, 0, IPC_RMID);

    std::cout << "\nAll exams have been marked" << std::endl;

    return 0;
}

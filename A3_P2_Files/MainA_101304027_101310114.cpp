
#include "PartA_101304027_101310114.h"
#include <unistd.h>
#include <iostream>
#include <sys/wait.h> 
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string>
#include <cstdlib>

int main(int argc, char* argv[]) {

    if (argc < 2){      //Ensuring proper function call
        std::cout << "Improper program call. Example of call: " << argv[0] << " <number_of_TAs>" << std::endl;
        return 1;
    }

    //Ensuring at least 2 TA's are created
    int num_TA = atoi(argv[1]);     
    if (num_TA < 2){    
        std::cout << "ERROR - Need at least two TA's" << std::endl;
        return 1;
    }

    // Creating Shared Memory 
    int shmid = shmget(IPC_PRIVATE, sizeof(shared_data), 0666 | IPC_CREAT);
    shared_data* var = (shared_data*)shmat(shmid, NULL, 0); 

    // Initializing Shared Data
    var->current_exam = 0;
    var->loader = 0; 
    load_rubric(var);
    load_exam(var);

    if (var->current_student == 9999){
        std::cout << "The first exam loaded into memory contains student number 9999, stopping all execution" << std::endl;
        shmdt(var);
        shmctl(shmid, IPC_RMID, nullptr);
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
            ta_process(i, var);
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
    std::cout << "\nAll exams have been marked" << std::endl;

    return 0;
}
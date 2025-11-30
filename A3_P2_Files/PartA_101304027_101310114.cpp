
#include "PartA_101304027_101310114.h"

void load_rubric(shared_data* var){
    std::ifstream rubric_file("rubric.txt");

    if (!rubric_file){
        std::cout << "ERROR - Rubric file could not be opened" << std::endl;
        return;
    }

    std::string line;

    for (int i = 0; i < NUM_RUBRIC; i++) {      //Loading contents of rubric file into shared memory by reading each line
        if (std::getline(rubric_file, line)) {
            strncpy(var->rubric[i], line.c_str(), RUBRIC_LINE_LEN);
            var->rubric[i][RUBRIC_LINE_LEN - 1] = '\0';
        } 
        else {
            std::cout << "ERROR - Rubric file has fewer than "<< NUM_RUBRIC << " lines." << std::endl;
            return;
        }
    }
    std::cout << "\nSUCCESS - Rubric file copied into shared memory" << std::endl;
}

void load_exam(shared_data* var){
    int exam_index = var->current_exam;

    std::string filename = "exam" + std::to_string(exam_index + 1) + ".txt";    //Creating a file name based on current index
    std::ifstream exam_file(filename);

    if (!exam_file){
        std::cout << "ERROR - Exam " << filename << " could not be opened" << std::endl;
        //var->current_student = 9999; 
        return;
    }

    std::string line;

    if (!std::getline(exam_file, line)){
        std::cout << "ERROR - Exam " << filename << " is empty" << std::endl;
        //var->current_student = 9999;  
        return;
    }

    for (int i = 0; i < NUM_QUESTIONS; i++) {   //Initializing questions to be unmarked
        var->questions_marked[i] = 0;    
    }

    var->current_student = std::stoi(line); //Converting first line of exam files to turn student number into integer for condition checking and printing

    if (var->current_student == 9999){
        std::cout << "File: " << filename << " has been loaded into shared memory" << std::endl;
        return;
    }

   std::cout << "File: " << filename << " has been loaded into shared memory" << std::endl;
}

void delay(int micro_seconds){  //Delay function used to implement the required delays and manage race conditions 
    int random_delay = micro_seconds + (rand() % (micro_seconds + 1));
    usleep(random_delay);
}

void save_rubric(shared_data* var, int ta_ID){
    std::ofstream rubric_file("rubric.txt");

    if (!rubric_file){
        std::cout << "ERROR - Rubric file could not be opened" << std::endl;
        return;
    }

    for (int i = 0; i <NUM_RUBRIC; i++){    //Taking modified rubric lines from shared memory and saving the changes into the rubric text file
        rubric_file << var->rubric[i] << std::endl;
    }
    std::cout << "TA: " << (ta_ID + 1) << " saved the changes to the rubric" << std::endl;
}

void modify_rubric(shared_data* var, int ta_ID, int line){

    char* comma = strchr(var->rubric[line], ',');   //Finding the postion of the comma in the current rubric line
    if (!comma){
        std::cout << "No comma found in rubric line " << (line + 1) << std::endl;
        return;
    }

    char* after_comma = comma + 1;  //Pointer to the charecter after the comma, used to locate the rubric value to modify

    while (*after_comma == ' '){    //Skipping possible spaces to locate the rubric value
        after_comma++;
    }    

    if (*after_comma == '\0'){      
        std::cout << "No rubric value found after comma in line " << (line + 1) << std::endl;
        return;
    }

    char current = *after_comma;    //After finding the current rubric value, it is stored for incrementing
    
    if (current >= 'A' && current <= 'Z'){  //Ensuring rubric values are only incremented to be uppercase letters
        if (current != 'Z'){
            *after_comma = current + 1;
        }
        else{
            *after_comma = 'A';
        }
    }
    else    //incorect charecter 
    {
        *after_comma = 'A';
    }

    std::cout << "TA: " << (ta_ID + 1) << " modified rubric line " << (line + 1) << " from '" << current << "' to '" << *after_comma << "'" << std::endl;

    save_rubric(var, ta_ID);
}

void load_next_exam(int ta_ID, shared_data* var){
    
    if (var->loader != 0) {     //If next exam is already being loaded by another TA, current TA stops loading
        return;  
    }
    
    var->loader = ta_ID + 1;  //If loader is free, current TA loads the next exam
    
    std::cout << "\nAll questions in exam " << (var->current_exam + 1) << " have been marked. TA: " << (ta_ID + 1)  << " is loading exam " << (var->current_exam + 2) 
    << " into shared memory" << std::endl;

    var->current_exam++;
    load_exam(var);

    if (var->current_student == 9999) {
        std::cout << "\nTA: " << (ta_ID + 1)  << " loaded exam with student number 9999, Stopping Execution.\n" << std::endl;
    } 
    
    std::cout << "" << std::endl; 
    var->loader = 0;  //Reseting variable to allow future TA's to load exams
}

void ta_process(int ta_ID, shared_data* var){
    srand(time(NULL) + ta_ID * 123);    //Ensuring each TA generates unique random values 

    int review_exam = -1;    //Variable to check if current TA has reviewed the rubric before marking the exam
    
    while(1){
        if (var->current_student == 9999){
            return;
        }

        while (var->loader != 0) {  //Waiting if another exam is being loaded
            usleep(20000);  
        }

        if (var->current_student == 9999){
            return;
        }

        if (review_exam != var->current_exam) {//Checking if current exam has been reviewed
            review_exam = var->current_exam;     //Updating review to indicate that the current exam is being reviewed
            
            std::cout << "TA: " << (ta_ID + 1)  << " is reviewing rubric before marking exam " << (var->current_exam + 1) << " for student " 
            << var->current_student << std::endl;

            // Step 1: TA is checking over the rubric and randomly modifying
            for (int i = 0; i < NUM_RUBRIC; i++){
                delay(500000);  

                if (rand() % 2){        //%50 chance to modify the rubric for each question
                    modify_rubric(var, ta_ID, i);
                }
            }

            std::cout << "TA: " << (ta_ID + 1)  << " finished reviewing the rubric and is now marking exam " << (var->current_exam + 1) 
            << " for student " << var->current_student << std::endl;
        }

        // Step 2: TA is marking questions
        while (1){
            if (var->current_student == 9999){
                return;
            }

            if (var->current_exam != review_exam){   //If new exam has been loaded, send TA to review the rubric
                break;
            }

            int marking_question = -1;  //Index of the question that the TA will mark, -1 means TA did not select a question for marking 

            for (int i = 0; i < NUM_QUESTIONS; i++){    // Trying to find an unmarked question
                if (var->questions_marked[i] == 0){
                    var->questions_marked[i] = -2; // TA selects question
                    marking_question = i;   //Storing index of the question the TA selected to mark
                    break;
                }
            }      

            if (var->current_exam != review_exam){   //if new exam has been loaded, send TA to review the rubric
                break;
            }

            if (marking_question == -1){    //Checking if all questions are marked
                bool all_marked = true;   
                
                for (int i = 0; i < NUM_QUESTIONS; i++){
                    if (var->questions_marked[i] != -1){
                        all_marked = false;
                        break;
                    }
                }
                
                if (all_marked){    //If all questions are marked, load next exam
                    load_next_exam(ta_ID, var);
                    if (var->current_student == 9999) {
                        return;
                    }
                    break;
                } 

                usleep(20000);  
                continue;
            }

            delay(1000000);  

            var->questions_marked[marking_question] = -1;   //Current Question is done being marked

            std::cout << "TA: " << (ta_ID + 1) << " marked question " << (marking_question + 1) << " on exam " << (var->current_exam + 1) << " for student " 
            << var->current_student << std::endl;
        }
    }
}
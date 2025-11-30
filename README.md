# SYSC4001_A3P2 

## Overview
This project simulates multiple TAs marking batches of exams concurrently, with a unique process being created for each TA. 

- **Part A**  
Uses shared memory to allow TAs access to data concurrently. However, the processes are not synchronized, and no semaphores are used. This results in race conditions, leading to undesired output.

- **Part B**  
Builds on Part A by introducing semaphores to synchronize access to shared memory. This manages race conditions, allowing the concurrent processes to be controlled.

## Files
* #### Part A)
- `mainA_101304027_101310114.cpp`
- `PartA_101304027_101310114.h`
- `PartA_101304027_101310114.cpp`
- `rubric.txt`
- `exam1.txt` – `exam20.txt`

* #### Part B)
- `mainB_101304027_101310114.cpp`
- `PartB_101304027_101310114.h`
- `PartB_101304027_101310114.cpp`
- `rubric.txt`
- `exam1.txt` – `exam20.txt`

## How to Compile and Run
Note: The value `2` represents the number of TAs passed. This value can be changed to any number ≥ `2`.

### Part A
```bash
g++ -o mainA_101304027_101310114 mainA_101304027_101310114.cpp PartA_101304027_101310114.cpp
./mainA_101304027_101310114 2
```
### Part B
```bash
g++ -o mainB_101304027_101310114 mainB_101304027_101310114.cpp PartB_101304027_101310114.cpp
./mainB_101304027_101310114 2
```
#### Additional Information)
- The first line of the exam text files contains the student number. A student of `9999` is used to end program execution for both parts. The student numbers of exam text files can be modified to change the output, and terminate the program at different exams. Currently, exam 20 contains `9999`.
  
- The rubric file contains the rubric values for each question that the TA's may randomly modify. These programs only work with rubric values between `A to Z`.

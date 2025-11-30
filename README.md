# SYSC4001_A3P2 

This project uses concurrent processes to simulate multiple TAs marking exams in parallel.

Part A acheives this by using shared memory, but does not use synchronization, or sempahores. Thus, the program runs into race condtions, affecting the output. 

Part B builds on this by introducing synchronization through the implementation of semaphores. This allowed us to minimize race condtions and meet the criteria of the project.

Part A Contains)
MainA.cpp
PartA_Functions.h
PartA_Functions.cpp
rubric.txt
exam1.txt - exam20.txt

Part B Contains)
MainB.cpp
PartB_Functions.h
PartB_Functions.cpp
rubric.txt
exam1.txt - exam20.txt

To Compile Part A)
g++ -o mainA_101304027_101310114 mainA_101304027_101310114.cpp PartA_101304027_101310114.cpp
./mainA_101304027_101310114 2              

To Compile Part B)
g++ -o mainB_101304027_101310114 mainB_101304027_101310114.cpp PartB_101304027_101310114.cpp
./mainB_101304027_101310114 2

*Note: the 2 represents the amount of TA's passed, this number can be modified to number equal to or greater than 2
*Note: The first line of the exam files contains the student number. A student of '9999' is used to end execution of both parts. The student numbers of exam file can be modified based on desired output, and program termination.
*Note: The rubric file contains the rubric values that the TA's may modify. These programs only work with rubric values between 'A' to 'Z.'

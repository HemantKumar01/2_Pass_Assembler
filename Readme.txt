/*****************************************************************************
AUTHOR: Hemant Kumar (2301CS20)
Declaration of Authorship
This is a project assignment of course CS2102 : Digital Logic and Computer Organization at CSE department IIT Patna. 
*****************************************************************************/

Files:
==========
*All the files i submitted, asm.cpp and claims.txt, have my name and student id at the start, along with a declaration of authorship.

The evidence for Assembler including the output files from the four test examples given (test1 to test4) and bubblesort has also been uploaded. They are test1(3 output files), test2(1 output file), test3(3 output files), 
test4(3 output files) and bubbleSort(3 output files) and five additional customTests (with output files). All the test files have been tested and work properly. 


Compilation instructions:
=========================

    g++ -std=c++11 -Wall -Wextra -pedantic asm.cpp -o asm
    ./asm <filename>

    	g++ -std=c++11 -Wall -Wextra -pedantic asm.cpp -o asm
    ./emu -[commands] filename.o

    Commands are 
    -trace  show instruction trace
    -read   show memory reads
    -write  show memory writes
    -before show memory dump before execution
    -after  show memory dump after execution
  	-wipe   wipe written flags before execution

    * provide the name of input file as argument while running the executable
    * Ouput files -> <filename>.log and if there are no errors then <filename>.o file and <filename>.lst file, where <filename> is the name of the input file provided in argument


Assumptions:
============
    * Mnemonic named “data” is always used at the end of assembly code.
    * Listing file (.lst) and Object file(.o) are generated only when there are no errors in the input Assembly code,  
      otherwise only Error file (.log) is generated.


=============================
Program format and structure:
=============================
1. The assembler:
    
    * Every instruction is implemented.
    * It contains a table of instruction names and expected operands.
    * It is a 2 pass assembler (implemented in asm.cpp)
    * outputs the required correct information in the form of three files
       *  .log file for errors and warnings
       *  .lst file for listing 
       *  .o file for machine code object
    * All important errors are diagnosed and corresponding error file gets generated (named <filename>.log)
    * consistent and sensible formatting with sensible program structure. 
    * sensible variable, function & type names with explanatory comments.
		* Advanced listing file gets generated. (named <filename>.lst)
		* assembles test programs (including bubble sort)
    * Machine code object file gets generated (named <filename>.o)
	  * SET instruction is also implemented (which carries extra marks).
    * Code is written in C++ with optimum time and space complexity.
    * Additional tests with outputs is also included


=============================
Testing:
=============================

1. The assembler:

I have tested  five examples(4 test files and bubblesort) shown as below. The errors only occurred in the file test2.asm and the program immediately stopped on the second pass. 

The others were tested without any errors detected and produced three output files, listing file, 
error log file and machine readable object file, for each example.

1)
#Input: test01.asm
#Output: 
a. test01.lst 
b. test01.log (without any error messages)
c. test01.o

2)
#Input: test02.asm
#Output: 
a. test02.log (with error messages shown as below and also in log file)

error messages:
Line: 4 ERROR: Duplicate label
Line: 6 ERROR: Invalid parameter
Line: 7 ERROR: Missing parameter
Line: 8 ERROR: Unexpected parameter
Line: 9 ERROR: Extra tokens
Line: 10 ERROR: Invalid label format
Line: 11 ERROR: Unknown command
Line: 12 ERROR: Unknown command


3)
#Input:asm test03.asm
#Output: 
a. test03.lst 
b. test03.log (without any error messages)
c. test03.o

4)
#Input: test04.asm
#Output: 
a. test04.lst 
b. test04.log (without any error messages)
c. test04.o

5)
#Input: MyBubbleSort.asm
#Output: 
a. bubbleSort.lst 
b. bubbleSort.log (without any error messages)
c. bubbleSort.o

Additional Tests:
=================
6) 
#Input: CustomTests/customTest01.asm
#Output: 
a. customTest01.lst 
b. customTest01.log (without any error messages)
c. customTest01.o

7) 
#Input: CustomTests/customTest02.asm
#Output: 
a. customTest02.lst 
b. customTest02.log (without any error messages)
c. customTest02.o

8) 
#Input: CustomTests/customTest03.asm
#Output: 
a. customTest03.lst 
b. customTest03.log (without any error messages)
c. customTest03.o

9) 
#Input: CustomTests/customTest04.asm
#Output: 
a. customTest04.lst 
b. customTest04.log (without any error messages)
c. customTest04.o

10) 
#Input: CustomTests/customTest05.asm
#Output: 
a. customTest05.lst 
b. customTest05.log (without any error messages)
c. customTest05.o
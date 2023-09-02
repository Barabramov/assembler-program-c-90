# assembler-program-c-90
This project deals with building assembler software in the C-90 language, for a defined assembler language- a detailed file in Hebrew inside (called Project-guidelines-hebrew.pdf) .

### The program is divided into three phases:
##### 1) Opening the input file and deploying the macro settings + create ".am" file.
   - pre_processor.c - the main file that manages this step.
   - macro_table.c - a file containing functions that operate on the data structure containing the macros.
     
##### 2) Create a data structure that stores all the symbols in the file and their details + count the number of instruction lines and the number of data lines.
- first_pass.c - a main file that manages and contains the functions that perform this step.

                  
##### 3) Converts the data and lines in the ".am" file to 12-bit binary codes. and additionally creates the required output files.
- second_pass.c - a main file that manages and contains the functions that perform this step.
###### Creates the following files-
  * ".ent" -  contains the symbols on which the ".entry" command was applied, and the address where the symbol is defined in the rest of the code.
  * ".ext" -  contains the symbols on which the ".extern" command was applied, and the addresses where the symbol is used in the rest of the code.
  * ".ob" - a word is 12 bits in size. Every 6 bits will be converted to an appropriate character according to the table defining 64Base. Each word will be added to the file on a separate line.
 
    
### Tools files:
 - main.c - The main code file of the project.
 -  utilities.c - Contains the functions dealing with the memory, general functions and the data structures.
 -  files.c - creat, open, and close the files.
 -  macros.c  
 -  makefile

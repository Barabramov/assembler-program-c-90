# assembler-program-c-90
This project deals with building assembler software in the C-90 language, for a defined assembler language- a detailed file in Hebrew inside (called Project-guidelines-hebrew.pdf) .

The program is divided into three phases:
1) Opening the input file and deploying the macro settings + create ".am" file.
   * pre_processor.c - the main file that manages this step.
   * macro_table.c - a file containing functions that operate on the data structure containing the macros.
2) Create a data structure that stores all the symbols in the file and their details + count the number of instruction lines and the number of data lines.
   * first_pass.c - 



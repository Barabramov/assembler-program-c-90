# Assembler Software (C-90)
This project involves the development of an assembler software written in C-90. The software processes a custom-defined assembler language. Detailed specifications can be found in the file [Project-guidelines-hebrew.pdf](Project-guidelines-hebrew.pdf) (Note: Documentation is in Hebrew).

## Implementation Overview:

The software operation is divided into three distinct phases:

### 1. Initial Processing:
* **Objective:** Open the input file, deploy macro settings, and generate an ".am" file.
  * **`pre_processor.c`**: Manages the operations of this phase.
  * **`macro_table.c`**: Contains functions that manipulate the data structure for macros.

### 2. Data Structure Creation:
* **Objective:** Construct a data structure to store all symbols from the file and their corresponding details. The phase also counts the number of instruction and data lines present.
  * **`first_pass.c`**: Manages and houses functions for this step.

### 3. Binary Code Generation:
* **Objective:** Convert data and lines in the ".am" file to 12-bit binary codes and subsequently create required output files.
  * **`second_pass.c`**: Oversees and contains functions for this step.
  
  **Generated Files:**
   * **`.ent`**: Lists symbols marked with the ".entry" command and the address where they're defined in the code.
   * **`.ext`**: Contains symbols marked with the ".extern" command and their usage addresses within the code.
   * **`.ob`**: Features 12-bit words, with every 6 bits being translated to a character as per the 64Base table definition. Each word gets its separate line in this file.

## Utility and Tool Files:

* **`main.c`**: Core file of the project.
* **`utilities.c`**: General utility functions, memory-related operations, and data structure functions reside here.
* **`files.c`**: Handles file creation, opening, and closure.
* **`macros.c`**: Macro-related functions.
* **`makefile`**: Compilation and build script for the project.

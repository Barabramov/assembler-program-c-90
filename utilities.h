# ifndef UTILITIES_H
# define UTILITIES_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_IN_LINE 81
#define MAX_SYMBOL_NAME 32
#define NUM_MIUN_METHODS 3
#define ACTIONS_NUM 16
#define DATA_NUM 4
#define REGISTERS_NUM 8
#define SPACE "\t\n "
#define COLON ": "
#define COMMA ", "
#define COMMENT "; "
#define LETTER_INDEX 26

typedef enum { false, true} bool;
typedef enum { instruction, data} line_type;



typedef struct symbol {
    struct symbol *next;
    char name[MAX_SYMBOL_NAME];
    int address;
    bool is_entry, is_data, is_external;
} symbol;


typedef enum {
    DATA_TYPE,
    STRING_TYPE,
    ENTRY_TYPE,
    EXTERN_TYPE
} data_type;


typedef enum {
    ACTION_TYPE,
    DIGIT_OR_SYMBOL_TYPE,
    REGISTER_TYPE
} use_type;


typedef struct{
    unsigned int a_r_e: 2;
    unsigned int dest_address: 3;
    unsigned int opcode: 4;
    unsigned int source_address: 3;
} action_machine_code;


typedef struct{
    unsigned int a_r_e: 2;
    unsigned int rest_address: 10;
} digit_or_symbol_machine_code;


typedef struct{
    unsigned int a_r_e: 2;
    unsigned int r1: 5;
    unsigned int r2: 5;
} register_machine_code;


typedef struct action_node {
    struct action_node *next;
    use_type bit;
    union{
        action_machine_code action;
        digit_or_symbol_machine_code digit_or_symbol;
        register_machine_code register_operand;
        unsigned int binary;
    }operand_type;
} action_node;



typedef struct data_node {
    struct data_node *next;
    unsigned int number :12;
} data_node;



/*
 * this struct define each action details.
 * opcode - the action code in decimal base.
 * source_operand- each index in the array represent "miun" method that the action can use as source (if 1- is true),
 * source_operands[0] is "miun" method number 1, source_operands[1] is "miun" method number 3, source_operands[2] is "miun" method number 5.
 * dest_operands- each index in the array represent "miun" method that the action can use as destination (if 1- is true),
 * the indexes represented the same as source_operand.
 */
typedef struct {
    char *name;
    int opcode;
    int total_operands_num;
    int source_operands[NUM_MIUN_METHODS];
    int dest_operands[NUM_MIUN_METHODS];
} operation;



/* this struct define each macro details. */
typedef struct macro_node {
    struct macro_node* next;
    char name[MAX_IN_LINE];
    /* lines - array list of each line that under the macro definition,*/
    char** lines;
    int line_counter;
    int line_capacity;
}macro_node;


/* this struct define macro_table details.
 * each table cell represent an english letter. */
typedef struct {
    macro_node* table[LETTER_INDEX];    
}macro_table;


/* this struct define symbol_table details.
 * each table cell represent an english letter. */
typedef struct {
    symbol* table[LETTER_INDEX];
}symbol_table;


extern symbol_table *curr_symbol_table;
extern macro_table *curr_macros_table;
extern char *registers[REGISTERS_NUM];
extern operation operations[ACTIONS_NUM];
extern char* data_types[DATA_NUM];


void int_to_12bit_binary(unsigned int value, char* output);
char *bits_to_base64(const char *bits, char *output);

/* free_s helps us to avoid common memory problems, gets the address of a pointer to free */
void free_s(void** memory);

/* malloc_s helps us to avoid common memory problems, gets size and give us pointer to the wright place. */
void* malloc_s (size_t size);

/* realloc_s helps us to avoid common memory problems, gets the pointer and find place according to the size input. */
void* realloc_s (void* memory, size_t size);

/* realloc_s helps us to avoid common memory problems, gets the pointer and find place according to the size input. */
void* calloc_s(size_t num_items, size_t item_size);

bool check_if_null_empty(char *token, char *string_to_print);

bool problems_alert(char *string_to_print,  int* curr_line);



# endif 

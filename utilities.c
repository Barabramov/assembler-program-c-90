#include "utilities.h"

const char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
/*
 * operations is a list of operations (actions and  their details).
 */
operation operations[] = {
        {"mov", 0,2, {1,1,1}, {0,1,1}},
        {"cmp", 1,2,{1,1,1}, {1,1,1}},
        {"add", 2,2, {1,1,1}, {0,1,1}},
        {"sub", 3,2, {1,1,1}, {0,1,1}},
        {"not", 4,1, {0,0,0}, {0,1,1}},
        {"clr", 5,1, {0,0,0}, {0,1,1}},
        {"lea", 6,2, {0,1,0}, {0,1,1}},
        {"inc", 7,1, {0,0,0}, {0,1,1}},
        {"dec", 8,1, {0,0,0}, {0,1,1}},
        {"jmp", 9,1, {0,0,0}, {0,1,1}},
        {"bne", 10,1, {0,0,0}, {0,1,1}},
        {"red", 11,1, {0,0,0}, {0,1,1}},
        {"prn", 12,1, {0,0,0}, {1,1,1}},
        {"jsr", 13,1, {0,0,0}, {0,1,1}},
        {"rts", 14,0, {0,0,0}, {0,0,0}},
        {"stop", 15,0, {0,0,0}, {0,0,0}}
};

char* data_types[] = {".data", ".string", ".entry", ".extern" };

char *registers[] = {"@r0", "@r1", "@r2", "@r3", "@r4", "@r5", "@r6", "@r7"};

macro_table *curr_macros_table = NULL;
symbol_table *curr_symbol_table = NULL;


bool check_if_null_empty(char *token, char *string_to_print){
    if (token == NULL)
    {
        printf("%s",string_to_print );
        return true;
    }
    return false;
}

bool problems_alert(char *string_to_print, int* curr_line){
    printf("%s%d.\n",string_to_print, *curr_line);
    return false;
}


char *bits_to_base64(const char *bits, char *output) {
    int i, value1 = 0, value2 = 0;
    for (i = 0; i < 6; i++) { value1 = value1 << 1 | (bits[i] - '0');}
    for (i = 6; i < 12; i++) { value2 = value2 << 1 | (bits[i] - '0');}

    output[0] = base64_table[value1 & 0x3f];
    output[1] = base64_table[value2 & 0x3f];
    output[2] = '\0';

    return output;
}

void int_to_12bit_binary(unsigned int value, char* output) {
    int i;
    for (i = 11; i >= 0; --i) {
        output[i] = (value & 1) ? '1' : '0';
        value >>= 1;
    }
    output[12] = '\0';

}

void free_s(void** memory)
{
    free(*memory);
    *memory = NULL;
}


void* malloc_s (size_t size)
{
    void* memory = malloc(size);
    if (memory == NULL)
    {
        perror("Error");
        exit(EXIT_FAILURE);
    }
    return memory;  
}



void* realloc_s (void* memory, size_t size)
{
    void* new_memory = realloc(memory, size);
    if (new_memory == NULL)
    {
        perror("Realloc Error"); 
        exit(EXIT_FAILURE);
    }
    return new_memory;
}

void* calloc_s(size_t num_items, size_t item_size)
{
    void* new_memory = calloc(num_items, item_size);
    if (new_memory == NULL)
    {
        perror("Error");
        exit(EXIT_FAILURE);
    }
    return new_memory;
}



# ifndef FIRST_H
# define FIRST_H

#include "main.h"



bool first_pass(FILE *input, int *ic, int *dc);
void update_address(int *ic);

bool is_comment_or_empty(char* token, int* line);
bool start_with_symbol(char** token, int* ic, int*dc, int* line, int* line_length);
bool in_symbol_definition(char* token);
bool check_symbol_name(char *name, int* line, int* line_length);
symbol *find_symbol(char *name);
bool add_symbol( char *name, int address, int is_data, int* line, int* line_length);
int count_char_occurrences( char *str, char this_ch);

int check_if_action(char* curr_word);
bool check_and_calc_action(int index, char* token,int *ic, int* line, int* line_length);

bool check_and_calc(int index, char* token,int *ic, int *dc, line_type curr_type, int* line, int* line_length);
int check_if_data(char* curr_word);
data_type get_data_type(const char* curr_data);
bool handle_data_type(char* token, int *dc, int* line, int* line_length);


bool is_digit (char* token, int* line, int print_error_or_not);
int is_it_register(char* token);


#endif




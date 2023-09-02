# ifndef PRE_H
# define PRE_H

#include "macro_table.h"
#include "macros.h"
#include "main.h"
void pre_processor_main (char *file_name);
macro_node * start_macro(char* token, int* is_in_macro, macro_node *curr_macro );
macro_node * not_in_macro(char curr_line[MAX_IN_LINE], int* is_in_macro, macro_node *curr_macro, FILE *output_file);
void add_macro_to_file(macro_node *macro, FILE *output_file);
#endif






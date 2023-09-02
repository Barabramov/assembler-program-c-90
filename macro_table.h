# ifndef MCRO_H
# define MCRO_H

#include "utilities.h"


macro_node* add_macro_node(char* name);
macro_node* search_macro(char* name);
void add_line_to_macro(macro_node* macro, char* line);
void free_macro_list(macro_node* head);
void free_macro_table();

#endif

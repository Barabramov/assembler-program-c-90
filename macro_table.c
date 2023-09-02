#include "macro_table.h"


macro_node* add_macro_node(char* name)
{
    int table_index;
    macro_node *curr_macro = NULL;
    if (name == NULL || !isalpha(name[0]) || strlen(name) >= MAX_IN_LINE) {
        /* Handle memory allocation error*/
        return NULL;
    }
    /* find the location in the hash table. */
    table_index = tolower(name[0]) - 'a';
    curr_macro = malloc_s(sizeof(macro_node));

    strcpy(curr_macro->name, name);
    curr_macro->line_counter = 0;
    curr_macro->line_capacity = 10;
    curr_macro->lines = malloc_s(curr_macro->line_capacity * sizeof(char *));
    if (!curr_macros_table) curr_macros_table = calloc_s(1, sizeof(macro_table));
    curr_macro->next = curr_macros_table->table[table_index];
    curr_macros_table->table[table_index] = curr_macro;
    return curr_macro;
}


void add_line_to_macro(macro_node* macro, char* line)
{
    if (macro->line_counter == macro->line_capacity) {
        macro->line_capacity *= 2 +1;
        macro->lines = realloc_s(macro->lines, macro->line_capacity * sizeof(char *));
    }
    macro->lines[macro->line_counter] = malloc_s(MAX_IN_LINE * sizeof(char));
    strcpy(macro->lines[macro->line_counter], line);
    macro->line_counter += 1;
}


/* search if we already saved the macro. */
macro_node* search_macro(char* name)
{
    int table_index;
    macro_node *macro = NULL;
    if (name == NULL || !isalpha(name[0]) || strlen(name) >= MAX_IN_LINE || !curr_macros_table ) {
        /* Handle memory allocation error*/
        return NULL;
    }
    /* find the location in the hash table. */
    table_index = tolower(name[0]) - 'a';
    macro = curr_macros_table->table[table_index];
    while (macro != NULL && strcmp(macro->name, name) != 0) {
        macro = macro->next;
    }
    return macro;
}


void free_macro_table()
{
    int i;
    if (curr_macros_table) {
        for (i = 0; i < LETTER_INDEX; i++) {
            free_macro_list(curr_macros_table->table[i]);
        }
        free_s((void **) &curr_macros_table);
    }
}


void free_macro_list(macro_node* head)
{
   while (head)
   {
       macro_node* temp = head;
       int i;
       for (i=0; i< head->line_counter; i++){free(head-> lines[i]);}
       free(head-> lines);
       head = head->next;
       free(temp);
   }

}


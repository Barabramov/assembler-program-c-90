#include "pre_processor.h"


void pre_processor_main (char *file_name) {
    char curr_line[MAX_IN_LINE];
    macro_node *curr_macro = NULL;
    FILE *input_file, *output_file;
    int is_in_macro = 0;

    /* open input file and checks open problems. */
    input_file = handle_with_file(file_name, ".as", "r");
    /* create the output file and checks open problems. */
    output_file = handle_with_file(file_name, ".am", "w");

    /* loop all lines. */
    while (fgets(curr_line, MAX_IN_LINE, input_file)) {
        /* remove new line sign. */
        curr_line[strcspn(curr_line, "\n")] = '\0';
        /* check if we still in definition of macro lines. */
        if (is_in_macro) {
            /* check if we need to end the definition. */
            if (strcmp(curr_line, "endmcro") == 0) is_in_macro = 0;
                /* still in macro definiton- add the line to macro. */
            else add_line_to_macro(curr_macro, curr_line);
        }
            /* not in macro definition. */
        else curr_macro= not_in_macro(curr_line,  &is_in_macro, curr_macro, output_file);
    }

    free_macro_table();
    /* close files and free the memory used. */
    fclose(output_file);
    fclose(input_file);
}





macro_node * not_in_macro(char curr_line[MAX_IN_LINE], int* is_in_macro, macro_node *curr_macro, FILE *output_file)
{
    macro_node *macro;
    char cpy_curr_line[MAX_IN_LINE], *token;

    /* create copy to curr_line- to not change the original line. */
    strcpy(cpy_curr_line, curr_line);
    token = strtok(cpy_curr_line, SPACE);
    if (token == NULL) return NULL;
    /* check if we get inside macro definition. */
    switch (strcmp(token, "mcro")) {
        /* into macro definition. */
        case 0:
            return start_macro(token, is_in_macro, curr_macro);
        /* we are not start a macro definition - we check if there is macro name uses. */
        default:
            macro = search_macro(token);
            /* if we found that a macro is used - add to output file the saved macro lines attribute. */
            if (macro != NULL)  add_macro_to_file(macro, output_file);
            /* if there is no macro name uses- add to output file the original line. */
            else fprintf(output_file, "%s\n", curr_line);
    }
    return NULL;

}


/* we enter to macro definition */
macro_node * start_macro(char* token, int* is_in_macro, macro_node *curr_macro )
{
    /* get next word - macro name. */
    token = strtok(NULL, SPACE);
    /* check if no macro name. */
    check_if_null_empty(token, "Error: macro name missing after 'mcro'\n");
    /* we start a new macro definition - we add it to the hash table, and update curr_macro and is_in_macro. */
    if(!search_macro(token))
    {
        *is_in_macro = 1;
        return add_macro_node(token);
    }
    return NULL;
}


void add_macro_to_file(macro_node *macro, FILE *output_file)
{
    int i;
    for (i = 0; i < macro->line_counter; i++)
    {
        fprintf(output_file, "%s\n", macro->lines[i]);
    }
}





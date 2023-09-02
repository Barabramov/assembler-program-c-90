#include "files.h"


FILE* handle_with_file(char* file_name, char add_extention[FILE_EXTENTION], char mode[2])
{
    FILE *input;
    /* Set file name size. */
    char *curr_file_name = malloc_s(strlen(file_name) + FILE_EXTENTION);;
    check_if_null_empty(curr_file_name, "Memory allocation failed for am_file_name.\n");
    /* set input file name with ext. */
    strcpy(curr_file_name, file_name);
    strcat(curr_file_name, add_extention);
    input = fopen(curr_file_name, mode);
    free_s((void **) &curr_file_name);
    if (!input) {
        perror("Failed to open the file");
        return NULL;
    }
    else return input;
}


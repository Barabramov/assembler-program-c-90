#include "main.h"


int main(int argc, char *argv[]) {
    /*char* file_name;*/
    FILE *input = NULL;
    int ic, dc, i;
    bool valid;

    /* check if file name is not provided. */
    if (argc < 2) {
        printf("Error: No file name provided.\n");
        return 0;
    }
    /* loop for each file in the command line*/
    for (i=1; i<argc; i++) {
        valid = true;
        pre_processor_main(argv[i]);

        ic = 100;
        dc = 0;

        input = handle_with_file(argv[i], ".am", "r");
        valid = first_pass(input, &ic, &dc);
        fclose(input);

        input = handle_with_file(argv[i], ".am", "r");
        second_pass(input, argv[i], &ic, &dc, valid);
        fclose(input);

    }
    return 0;
}


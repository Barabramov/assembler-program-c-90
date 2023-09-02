#include "first_pass.h"


bool first_pass(FILE *input, int *ic, int *dc) {
    /* initialize parameters. */
    char curr_line[MAX_IN_LINE],cpy_curr_line[MAX_IN_LINE], *token;
    int  curr_action_index, curr_data_index, line=0, line_length;
    bool is_valid = true;

    /* open input file and checks if there's open problems. */
    curr_symbol_table = calloc_s(1, sizeof(symbol_table));
    /* loop all lines. */
    while (fgets(curr_line, MAX_IN_LINE, input)) {
        line ++;
        line_length = 0;
        /* remove new line sign. */
        curr_line[strcspn(curr_line, "\n")] = '\0';
        strcpy(cpy_curr_line, curr_line);
        token = strtok(cpy_curr_line, SPACE);
        /* check if empty or comment line. */
        if (!is_comment_or_empty(token, &line)) continue;
        if (!start_with_symbol(&token, ic, dc, &line, &line_length)) {
            is_valid = false;
            continue;
        }
        /* check if the current word is action or data. */
        curr_data_index = check_if_data(token);
        curr_action_index = check_if_action(token);
        if (curr_action_index != 0) is_valid = check_and_calc(curr_action_index-1,token,  ic, dc, instruction, &line, &line_length);
        else if (curr_data_index != 0 )  is_valid =  check_and_calc(curr_data_index-1, token, ic, dc, data, &line, &line_length);
        else  is_valid = problems_alert("Error: no instruction/data as expected in line- ", &line);

    }

    update_address(ic);
    return is_valid;

}


bool is_comment_or_empty(char* token, int* line)
{
    if (token == NULL) return problems_alert("PASS LINE: this line is an empty line. in line- " , line);
    else if (token[0] == COMMENT[0]) return problems_alert("PASS LINE: this line is a comment line. in line- " , line);
    return true;
}


/* if the first word is a definition of symbol- add the symbol to symbol_table and check next word */
bool start_with_symbol(char** token, int* ic, int*dc, int* line, int* line_length)
{
    int is_data;
    char * cpy_token;
    if (in_symbol_definition(*token))
    {
        cpy_token = *token;
        *token = strtok(NULL, SPACE);
        if (*token == NULL) return problems_alert("Error: empty symbol definition, in line- " , line);
        is_data = check_if_data(*token);
        /* if after the symbol exist entry or extern pass the line. */
        if (strcmp(".entry", *token) == 0 || strcmp(".extern", *token) == 0 ) return !problems_alert("PASS SYMBOL: Ignoring the symbol, in line- " , line);
        else return add_symbol( cpy_token,  is_data ? *dc : *ic , is_data, line, line_length);
    }
    return true;
}



/* check if we in definition of symbol */
bool in_symbol_definition(char* token){
    char* check_colon;
    /* search the colon from the end of the token. */
    check_colon = strrchr(token, COLON[0]);
    /* if colon is exists in the word. */
    if (check_colon)
    {
        /* check if the colon char is at the end of the symbol. */
        if ((check_colon-token == strlen(token)-1)) return true;
    }
    return false;
}


/* adds the symbol to the symbol table */
bool add_symbol(char *token, int curr_address, int is_data, int* line, int* line_length) {
    int table_index;
    symbol *curr_symbol;
    char* symbol_name = token;

    /* if not external, find symbol name without the colon char. for sure there is no space in the middle, because it is token that separated by spaces.*/
    if ((is_data != EXTERN_TYPE && is_data) || !is_data) symbol_name[strlen(symbol_name)-1] = '\0';
    /* check the validation of the symbol name. */
    if (!check_symbol_name(symbol_name, line, line_length)) return false;
    (*line_length) += (int)strlen(symbol_name);
    /* find the location in the hash table. */
    table_index = tolower(symbol_name[0]) - 'a';
    curr_symbol = malloc_s(sizeof(symbol));
    /* definition of all the details in "symbol" and in "symbol_table". */
    strcpy(curr_symbol->name, symbol_name);
    curr_symbol->address = curr_address;
    if (is_data == EXTERN_TYPE) curr_symbol->is_external = true;
    else curr_symbol->is_external = false;
    if (is_data>0) curr_symbol->is_data = true;
    else curr_symbol->is_data = false;
    curr_symbol->is_entry = false;
    curr_symbol->next = curr_symbol_table->table[table_index];
    curr_symbol_table-> table[table_index] = curr_symbol;
    return true;
}


/* adds to all the data_nodes addresses the ic number */
void update_address(int *ic)
{
    int i;
    symbol *curr_symbol;
    for (i=0; i<LETTER_INDEX; i++){
        curr_symbol = curr_symbol_table-> table[i];
        while (curr_symbol)
        {
            if (curr_symbol->is_data && !(curr_symbol->is_external)) curr_symbol->address += (*ic);
            curr_symbol =  curr_symbol->next;
        }
    }
}



/* check if the symbol is already exists in the symbol table */
symbol *find_symbol(char *name) {
    int table_index;
    symbol *s;
    /* check if the symbol name that we get is not valid. */
    if (name == NULL || !isalpha(name[0]) || strlen(name) > MAX_SYMBOL_NAME) {
        return NULL;
    }
    /* find the location in the hash table. */
    table_index = tolower(name[0]) - 'a';
    s = curr_symbol_table->table[table_index];
    while (s != NULL && strcmp(s->name, name) != 0) {
        s = s->next;
    }
    return s;
}



/* check if it exists in the action table. */
int check_if_action(char* curr_word) {
    int i;
    for (i=0; i<ACTIONS_NUM;i++) {
        char* curr_name = operations[i].name;
        if (strcmp(curr_name, curr_word) == 0) return i+1;
    }
    return 0;
}


/* check if it exists in the data table */
int check_if_data(char* curr_word) {
    int i;
    for (i=0; i<DATA_NUM;i++) {
        char* curr_name = data_types[i];
        if (strcmp(curr_name, curr_word) == 0) return i+1;
    }
    return 0;
}


bool check_and_calc(int index, char* token,int *ic, int *dc, line_type curr_type, int* line, int* line_length) {
    char *curr_data;
    if (curr_type) {
        curr_data = data_types[index];
        (*line_length)+= (int)strlen(curr_data);
        switch(get_data_type(curr_data)) {
            /* if the sentence type is a string - add the length of the string to dc counter. */
            case STRING_TYPE:
                (*line_length)+= strlen(".string");
                token = strtok(NULL, "\"");
                if (token == NULL) return problems_alert("Error: there is no string  after \".string\". in line- " , line);
                (*line_length)+= (int)strlen(token)+1 ;
                if ( (*line_length) >= MAX_IN_LINE) return problems_alert("Error: this line goes outside the max line length. in line- " , line);
                (*dc) += (int)(strlen(token) + 1);
                break;

            case DATA_TYPE:
                return handle_data_type(token, dc, line,line_length);

            case EXTERN_TYPE:
                token = strtok(NULL, SPACE);
                return add_symbol( token, 0, index, line, line_length);

            case ENTRY_TYPE:
                break;
        }
        return true;
    }
    else return check_and_calc_action(index, token,ic, line ,line_length );
}




bool check_and_calc_action(int index, char* token,int *ic, int* line, int* line_length)
{
    int commas_counter=0;
    operation *curr_op;
    curr_op = operations + index;
    (*ic)++;
    token = strtok(NULL, "");
    if(token != NULL) commas_counter = count_char_occurrences(token, COMMA[0]);
    (*line_length)+= (int)(strlen(curr_op->name));
    switch (curr_op->total_operands_num) {
        case 0:
            if (token != NULL) return problems_alert("Error: there is operand after instruction (as not expected). in line- " , line);
            break;

        case 1:
            if (commas_counter != 0) return problems_alert("Error: the number of commas/operands is incorrect. in line - ", line);
            token = strtok(token, SPACE);
            if (token == NULL) return problems_alert("Error: there is less then 1 operand after instruction (as expected). in line- " , line);
            (*ic)++;
            (*line_length)+= (int)strlen(token);
            break;

        case 2:
            token = strtok(token, SPACE);
            if (token == NULL || commas_counter != 1) return problems_alert("Error: the number of commas/operands is incorrect. in line - ", line);
            (*line_length)+= (int)strlen(token);
            if (strlen(token)== REGISTERS_LEN && token[strlen(token)-1]==COMMA[0]) token[strlen(token)-1]='\0';
            if (is_it_register(token)) {
                token = strtok(NULL, SPACE);
                if (token == NULL) return problems_alert("Error: the number of commas/operands is incorrect. in line - ", line);
                (*line_length) += (int) strlen(token);
                if ((*line_length) >= MAX_IN_LINE) return problems_alert("Error: this line goes outside the max line length. in line- ", line);
                if (is_it_register(token)) (*ic)++;
                else(*ic)+=2;
            } else (*ic)+=2;
            break;
    }
    return true;
}


/* data line */
bool handle_data_type(char* token,  int *dc, int* line, int* line_length){
    int number_of_commas, num_counter = 0;
    token = strtok(NULL, "");
    number_of_commas= count_char_occurrences(token, COMMA[0]);
    token = strtok(token, COMMA);
    /* no comma, so it check the whole part after ".data" . */
    if (token == NULL) return problems_alert("Error: need to be at least one number after \".data\". in line- " , line);
    while (token){
        num_counter ++;
        if(is_digit(token, line, 1)){
            (*dc)++;
            (*line_length) += (int) strlen(token);
            if ( (*line_length) >= MAX_IN_LINE) return problems_alert("Error: this line goes outside the max line length. in line- " , line);
        }
        else return false;
        token = strtok(NULL, COMMA);
    }
    if (number_of_commas >= num_counter)  return problems_alert("Error: comma's problem in line- " , line);
    return true;
}


int count_char_occurrences(char *str, char this_ch) {
    int count = 0, i;
    for (i = 0; i < strlen(str); i++) {
        if (str[i] == this_ch) count++;
    }
    return count;
}



data_type get_data_type(const char* curr_data) {
    if (strcmp(curr_data, ".string") == 0) return STRING_TYPE;
    if (strcmp(curr_data, ".data") == 0) return DATA_TYPE;
    if (strcmp(curr_data, ".extern") == 0) return EXTERN_TYPE;
    if (strcmp(curr_data, ".entry") == 0) return ENTRY_TYPE;
    else return 0;
}


/* check if it digit. */
bool is_digit (char* token, int* line, int print_error_or_not){
    int i = 0;
    while ((token[i] == '\t' || token[i] == ' ' || token[i] == '\0') && strlen(token) > i) { i++;}
    if ((strlen(token) == 0 || strlen(token) == i) && print_error_or_not)  return problems_alert("Error: missing digit. in line- " , line);
    if (token[i] == '+' || token[i] == '-') {
        if ((strlen(token)-i == 1) && print_error_or_not) return problems_alert("Error: argument start with minus/plus but without number after, in line- " , line);
        i++;
    }
    for ( ; i<strlen(token); i++){
        if (!isdigit((unsigned char)token[i])) {
            if (token[i] == '\t' || token[i] == ' ') break;
            else {if (print_error_or_not) return problems_alert("Error: the digit parameter includes a non-numeric character, in line- " , line);}
        }
    }
    for ( ; i<strlen(token); i++) { if (token[i] != '\t' && token[i] != ' ' && print_error_or_not)  return problems_alert("Error: the digit parameter separated by tab/space, in line- " , line); }
    return true;
}


/* check if it register. return the index of register in registers[]. */
int is_it_register(char* token)
{
    int i;
    for (i=0; i< REGISTERS_NUM; i++)
    {
       if (strcmp(token, registers[i]) ==0) return i+1;
    }
    return 0;
}


/* check the validation of symbol name. */
bool check_symbol_name(char *name, int* line, int* line_length ) {
    int i;
    if (name == NULL || !isalpha(name[0]) || strlen(name) > MAX_SYMBOL_NAME || find_symbol(name) || (*line_length)+ strlen(name)+1 >= MAX_IN_LINE)  return problems_alert("Error: symbol is not valid. in line- " , line);
    for (i = 1; i < strlen(name); i++) { if (!isalpha(name[i]) && !isdigit(name[i])) return problems_alert("Error: symbol is not valid. contains non-alphabetic and non-numeric character. in line- " , line); }
    for (i = 0; i < REGISTERS_NUM; i++) { if (strcmp(name, (registers[i]+sizeof(char))) == 0 )  return problems_alert("Error: symbol is not valid. equals to register name, in line- " , line); }
    for (i = 0; i < DATA_NUM; i++) { if (strcmp(name, data_types[i]+1) == 0 ) return problems_alert("Error: symbol is not valid. equals to data definition, in line- " , line); }
    if (check_if_action(name) > 0) return problems_alert("Error: symbol is not valid. equals to action name, in line- " , line);
    return true;
}


#include "second_pass.h"


bool second_pass(FILE *input, char *file_name, int *ic, int* dc, bool first_valid) {
    /* initialize parameters. */
    char curr_line[MAX_IN_LINE],cpy_curr_line[MAX_IN_LINE], *token;
    symbol *curr_symbol = NULL;
    int this_ic = 100, line=0;
    bool is_valid = true;
    data_node * last_data_node = NULL, *first_data_node = NULL ;
    action_node *last_action_node = NULL,  *first_action_node = NULL;

    /* loop all lines. */
    while (fgets(curr_line, MAX_IN_LINE, input)) {
        line++;
        /* remove new line sign. */
        curr_line[strcspn(curr_line, "\r\n")] = '\0';
        strcpy(cpy_curr_line, curr_line);
        /* gets first word in the current copy line. */
        token = strtok(cpy_curr_line, SPACE);
        if (strcmp(token,";")==0) continue;
        if (token[strlen(token)-1] == COLON[0]) token[strlen(token)-1] = '\0';
        curr_symbol = find_symbol(token);
        /* check if the first word is symbol. */
        if (curr_symbol)   is_valid = first_word_symbol(curr_symbol, token, &last_data_node, &last_action_node, &first_data_node, &first_action_node, &this_ic, file_name, &line);
        else if (check_if_action(token)) is_valid = handle_action_line(token,&last_action_node,  &first_action_node, &this_ic, file_name, &line);
        else if (get_data_type(token) == ENTRY_TYPE)
        {
            token = strtok(NULL, SPACE);
            curr_symbol = find_symbol(token);
            if (curr_symbol)
            {
                if (curr_symbol->is_external) is_valid = problems_alert("Error: entry an symbol to file that already defined as external symbol. this is not valid, in line- ", &line);
                else { (curr_symbol->is_entry) = true;}
            }
            else is_valid = problems_alert("Error: entry an symbol to file that is not define inside. in line- ", &line);
        }
    }
    if (!is_valid || !first_valid) return is_valid;
    object_file(file_name, first_action_node, first_data_node, ic, dc);
    ent_file(file_name);
    close_ext_file( NULL , 1);
    free_data_and_action_nodes( &last_data_node, &last_action_node, &first_data_node, &first_action_node);
    free_symbol_table();
    free_s((void **) &curr_symbol);
    return is_valid;


}

/* create the .ob file and go over the array lists.
 * convert the attribute to binary code and then to base64. */
void object_file(char *file_name, action_node *first_action_node, data_node *first_data_node,  int *ic, int* dc)
{
    FILE *output_file;
    char bits[13] , *output = malloc_s(3);
    data_node *curr_data_node = first_data_node;
    action_node *curr_action_node = first_action_node;
    output_file = handle_with_file(file_name, ".ob", "w");
    fprintf(output_file, "%d %d\n", (*ic)-100, *dc);
    while (curr_action_node)
    {
        int_to_12bit_binary(curr_action_node->operand_type.binary, bits);
        output = bits_to_base64(bits, output);
        fprintf(output_file, "%s\n", output);
        curr_action_node = curr_action_node->next;
    }
    while (curr_data_node)
    {
        int_to_12bit_binary(curr_data_node->number, bits);
        output = bits_to_base64(bits, output);
        fprintf(output_file, "%s\n", output);
        curr_data_node = curr_data_node->next;
    }
    fclose(output_file);
    free_s((void **) &output);
}

void free_data_and_action_nodes( data_node **last_data_node, action_node **last_action_node, data_node** first_data_node, action_node** first_action_node){
    while (*first_action_node) {
        *last_action_node = (*first_action_node)->next;
        free_s((void **) first_action_node);
        *first_action_node = *last_action_node;
    }
    while (*first_action_node) {
        *last_data_node = (*first_data_node)->next;
        free_s((void **) first_data_node);
        *first_data_node = *last_data_node;
    }
}

void free_symbol_table() {
    int i;
    symbol *curr_symbol, *prev_symbol;
    for (i = 0; i < LETTER_INDEX; i++) {
        curr_symbol = curr_symbol_table->table[i];
        while (curr_symbol) {
            prev_symbol = curr_symbol;
            curr_symbol = curr_symbol->next;
            free_s((void **) &prev_symbol);
        }
    }
    free_s((void **) &curr_symbol_table);
}

/* create the .ent file and go over the symbol_table.
 * add to file the symbol that the entry flag is true. */
void ent_file(char *file_name)
{
    int i, symbol_count, exist_symbol;
    FILE *output_file;
    symbol *curr_symbol;

    for (i=0,symbol_count=0, exist_symbol=0; i<LETTER_INDEX; i++){
        curr_symbol = curr_symbol_table-> table[i];
        while (curr_symbol) {
            if (curr_symbol->is_entry == true ) {
                if (symbol_count == 0) output_file = handle_with_file(file_name, ".ent", "w");
                fprintf(output_file, "%s  %d\n", curr_symbol->name, curr_symbol->address);
                symbol_count++;
                exist_symbol = 1;
            }
            curr_symbol = curr_symbol->next;
        }
    }
    if (exist_symbol) fclose(output_file);
}



/* check if the line that start with a symbol is data or instruction. */
bool first_word_symbol(symbol *curr_symbol, char *token, data_node **last_data_node, action_node **last_action_node, data_node** first_data_node, action_node** first_action_node, int *ic, char* file_name, int* line)
{
    token = strtok(NULL, SPACE);
    if (curr_symbol->is_data)
    {
        handle_data_line(token, last_data_node,  first_data_node);
        return true;
    }
    else
    {
        return handle_action_line(token,last_action_node, first_action_node, ic,file_name, line);
    }
}




void handle_data_line(char* token, data_node** last_data_node, data_node** first_data_node)
{
    int  i;
    char curr_char;
    data_node *curr_data_node = calloc_s(1, sizeof(data_node));

    switch(get_data_type(token)){
        case STRING_TYPE:
            token = strtok(NULL, SPACE);
            for (i=1; i < strlen(token)-1; i++){
                curr_char = token[i];
                curr_data_node-> number = (unsigned int)curr_char;
                insert_data(curr_data_node,last_data_node,  first_data_node);
                curr_data_node = calloc_s(1, sizeof(data_node));
            }
            curr_data_node-> number = (unsigned int)'\0';
            insert_data(curr_data_node,last_data_node,  first_data_node);
            break;

        case DATA_TYPE:
            handle_case_data_type(token, last_data_node, first_data_node) ;
            break;

        case ENTRY_TYPE:
        case EXTERN_TYPE:
            break;
    }
}


void handle_case_data_type(char* token, data_node** last_data_node, data_node** first_data_node) {
    char *copy_token;
    data_node* curr_data_node;
    copy_token = strtok(NULL, COMMA);
        /* give us the first number in data . */
        while (copy_token) {
            curr_data_node = calloc_s(1, sizeof(data_node));
            binary_code_data_line(copy_token,  last_data_node, first_data_node, &curr_data_node);
            copy_token = strtok(NULL, COMMA);
        }
}



/* handle action line according the action type. the types is according the number of the operands that the action have. */
bool handle_action_line(char* token, action_node** last_action_node, action_node** first_action_node, int *ic, char* file_name, int* line) {
    operation *curr_op = NULL;
    int index, num_of_operands;
    index = check_if_action(token) - 1;
    curr_op = (operation*)(operations + index);
    num_of_operands = curr_op->total_operands_num;
    switch (num_of_operands) {
        case 0:
            binary_code_first_line(0, 0, curr_op->opcode, 0, last_action_node,  first_action_node, ic);
            break;

        case 1:
            return handle_one_operand(curr_op,  token, last_action_node,  first_action_node, ic, file_name, line);

        case 2:
           return  handle_two_operands(curr_op, last_action_node,  first_action_node , ic, file_name, line);

        default:
            break;
    }
    return true;
}


/* if action have only one operand. */
bool handle_one_operand(operation *curr_op, char* token, action_node** last_action_node, action_node** first_action_node, int *ic, char* file_name, int* line)
{
    int operand_number, last_was_register;
    token = strtok(NULL, SPACE);
    operand_number = operand_type(token, line);
    if (!check_operands_validation(curr_op, operand_number, 0)) return problems_alert("Error: operand type is not valid. in line- ", line);
    binary_code_first_line(0, operand_number, curr_op->opcode, 0, last_action_node,first_action_node, ic);
    last_was_register = 0;
    switch_types_operands(token, &last_was_register, last_action_node,first_action_node, 0, ic, file_name, line);
    return true;
}



/* if action have two operands. */
bool handle_two_operands(operation *curr_op, action_node** last_action_node, action_node** first_action_node, int *ic, char* file_name, int* line)
{
    char *copy_operands, *next_copy_token;
    int last_was_register;
    copy_operands = strtok(NULL, COMMA);
    if (!check_operands_validation(curr_op, operand_type(copy_operands, line), 1)) return problems_alert("Error: operand type is not valid. in line- ", line);
    next_copy_token = strtok(NULL, COMMA);
    if (next_copy_token == NULL) return problems_alert("Error: no second operand. in line- ", line);
    if (next_copy_token[0]==COMMA[0]) next_copy_token = next_copy_token+1;
    if ( !check_operands_validation(curr_op, operand_type(next_copy_token, line), 0)) return problems_alert("Error: operand type is not valid. in line- ", line);
    binary_code_first_line(0, operand_type(next_copy_token, line), curr_op->opcode, operand_type(copy_operands, line), last_action_node, first_action_node, ic);
    last_was_register = 0;
    switch_types_operands(copy_operands, &last_was_register,last_action_node, first_action_node, 0, ic, file_name, line);
    switch_types_operands(next_copy_token, &last_was_register, last_action_node, first_action_node, 1, ic, file_name, line);
    return true;
}


/* here we find the type of the operand for knowing how to handle with it. */
void switch_types_operands(char* token, int *last_was_register, action_node** last_action_node, action_node** first_action_node, int round, int *ic, char* file_name, int* line)
{
    symbol *curr_symbol;
    if (round == 0 && token[strlen(token)-2]== COMMA[0]) token[strlen(token)-2]= '\0';
    switch (operand_type(token, line)) {
        case 1:
            binary_code_digit_or_symbol(0, atoi(token), last_action_node, first_action_node, ic, file_name, NULL);
            break;
        case 3:
            curr_symbol = find_symbol(token);
            if (curr_symbol->is_external) binary_code_digit_or_symbol(1, 0, last_action_node,first_action_node, ic, file_name, curr_symbol->name);
            else binary_code_digit_or_symbol(2, curr_symbol->address, last_action_node, first_action_node, ic, file_name, curr_symbol->name);
            break;

        case 5:
            if (*last_was_register)
                ((*last_action_node)->operand_type.register_operand.r1) = (is_it_register(token) - 1);
            else
            {
                second_register(last_action_node, token, first_action_node, ic);
                (*last_was_register)= 1;
            }
            break;

        default:
            break;
    }
}


/* check if the operand type is match to the action definition. */
bool check_operands_validation(operation *curr_op, int operand_number, int source)
{
    if (source) {
        if (!curr_op->source_operands[(int)((operand_number-1)/2)]) return false;
    }
    else
    {
        if (!curr_op->dest_operands[(int)((operand_number-1)/2)]) return false;
    }
    return true;
}


void second_register(action_node** last_action_node, char* token, action_node** first_action_node, int *ic)
{
    action_node* next_action_node;
    next_action_node = calloc_s(1, sizeof (action_node));
    next_action_node->operand_type.register_operand.a_r_e = 0;
    next_action_node->operand_type.register_operand.r2 = is_it_register(token) - 1;
    next_action_node->operand_type.register_operand.r1 = 0;
    next_action_node-> bit = REGISTER_TYPE;
    insert_action(next_action_node, last_action_node, first_action_node, ic);
}

int operand_type(char* token, int* line)
{
    if (find_symbol(token)) return 3;
    else if (is_it_register(token)) return 5;
    else if (is_digit(token, line, 0)) return 1;
    else return problems_alert("Error: operand type is not familiar. in line- ", line);
}


void binary_code_data_line(char* binary_num, data_node** last_data_node, data_node** first_data_node, data_node** curr_data_node)
{
    (*curr_data_node)->number = atoi(binary_num);
    insert_data(*curr_data_node, last_data_node, first_data_node);
}


/* insert all the details of the first line to the current action node. */
void binary_code_first_line(unsigned int a_r_e, unsigned int dest_address, unsigned int opcode, unsigned int source_address, action_node** last_action_node, action_node** first_action_node, int *ic)
{
    action_node *curr_action_node = malloc_s(sizeof(action_node));
    curr_action_node->operand_type.action.a_r_e = a_r_e;
    curr_action_node->operand_type.action.opcode = opcode;
    curr_action_node->operand_type.action.source_address = source_address;
    curr_action_node->operand_type.action.dest_address = dest_address;
    curr_action_node->bit = ACTION_TYPE;
    insert_action(curr_action_node, last_action_node, first_action_node, ic);
}


/* insert all the details of digit or symbol to the current action node. */
void binary_code_digit_or_symbol(unsigned int a_r_e, unsigned int rest_address, action_node** last_action_node, action_node** first_action_node, int *ic, char* file_name, char* curr_symbol_name)
{
    static FILE *output_file = NULL;
    action_node *curr_action_node = malloc_s(sizeof(action_node));
    static int exist_external = 0;
    if (a_r_e == 1) {
        if (!exist_external)
        {
            output_file = handle_with_file(file_name, ".ext", "w");
            close_ext_file(output_file, 0);
        }
        if (curr_symbol_name){
            fprintf(output_file, "%s  %d\n", curr_symbol_name, *ic);
            exist_external = 1;
        }

    }
    curr_action_node->operand_type.digit_or_symbol.a_r_e = a_r_e;
    curr_action_node->operand_type.digit_or_symbol.rest_address  = rest_address;
    curr_action_node->bit = DIGIT_OR_SYMBOL_TYPE;
    insert_action(curr_action_node, last_action_node, first_action_node, ic);
}


void close_ext_file( FILE *file, int to_free)
{
    static FILE *output_file = NULL;
    if (!to_free) output_file = file;
    if (to_free && output_file) fclose(output_file);
}



void insert_action(action_node *curr_action_node, action_node** last_action_node, action_node** first_action_node, int *ic)
{
    curr_action_node->next= NULL;
    if (*last_action_node)  {
        (*last_action_node)->next = curr_action_node;
        (*last_action_node) = curr_action_node;
    }
    else {
        *first_action_node = curr_action_node;
        *last_action_node = *first_action_node;
    }

    (*ic)+=1;
}


void insert_data(data_node *curr_data_node, data_node** last_data_node, data_node** first_data_node)
{
    curr_data_node->next= NULL;
    if (*last_data_node)
    {
        (*last_data_node)->next = curr_data_node;
        (*last_data_node) = curr_data_node;

    }

    else {
        *first_data_node = curr_data_node;
        *last_data_node =  *first_data_node;
    }
}
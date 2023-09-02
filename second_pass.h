#ifndef SECOND_PASS_H
#define SECOND_PASS_H

#include "first_pass.h"
#include "utilities.h"
#include "macro_table.h"
#include "pre_processor.h"
#include "macros.h"

bool second_pass(FILE *input, char *file_name, int *ic, int *dc,  bool first_valid);
bool first_word_symbol(symbol *curr_symbol, char *token, data_node **last_data_node, action_node **last_action_node, data_node** first_data_node, action_node** first_action_node, int *ic, char* file_name, int* line);
void object_file(char *file_name,action_node *first_action_node, data_node *first_data_node,  int *ic, int* dc);
void ent_file(char *file_name);
void close_ext_file( FILE *file, int to_free);
bool check_operands_validation(operation *curr_op, int operand_number, int source);

void handle_data_line(char* token, data_node** last_data_node, data_node** first_data_node);
void handle_case_data_type(char* token, data_node** last_data_node, data_node** first_data_node);
void binary_code_data_line(char* binary_num, data_node** last_data_node, data_node** first_data_node, data_node** curr_data_node);

void insert_data(data_node *curr_data_node, data_node** last_data_node, data_node** first_data_node);


bool handle_action_line(char* token, action_node** last_action_node, action_node** first_action_node, int *ic, char* file_name, int* line);

int operand_type(char* token, int* line);
void switch_types_operands(char* token, int *last_was_register, action_node** last_action_node,action_node** first_action_node, int round,int *ic, char* file_name,  int* line);
bool handle_one_operand(operation *curr_op,  char* token, action_node** last_action_node, action_node** first_action_node,  int *ic, char* file_name, int* line);
bool handle_two_operands(operation *curr_op, action_node** last_action_node, action_node** first_action_node, int *ic, char* file_name, int* line);
void second_register(action_node** last_action_node, char* token, action_node** first_action_node, int *ic);

void binary_code_first_line(unsigned int a_r_e, unsigned int dest_address, unsigned int opcode, unsigned int source_address, action_node** last_action_node, action_node** first_action_node, int *ic);
void binary_code_digit_or_symbol(unsigned int a_r_e, unsigned int rest_address, action_node** last_action_node, action_node** first_action_node, int *ic, char* file_name, char* curr_symbol_name);

void insert_action(action_node *curr_action_node, action_node** last_action_node, action_node** first_action_node, int *ic);

void free_data_and_action_nodes( data_node **last_data_node, action_node **last_action_node, data_node** first_data_node, action_node** first_action_node);
void free_symbol_table();

#endif
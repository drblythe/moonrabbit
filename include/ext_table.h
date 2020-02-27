#ifndef EXT_TABLE_H
#define EXT_TABLE_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct ext_node {
	char *data;
	struct ext_node* next;
} ext_node;

// 
typedef struct ext_linked_list{
	struct ext_node* head;
	char* title;
	bool exec_in_term;
} ext_linked_list;

// An array of linked lists to store program paths and their associated file extensions (from the config file).
typedef struct ext_table {
	unsigned int size;
	unsigned int capacity;
	ext_linked_list* list;
} ext_table;

//	 	Default table size is 10
int 	ext_table_init(ext_table* c_table);
// 		Doubles table when it has reached capacity
int 	ext_table_grow(ext_table* c_table);
//  	Insert a new program (a new linked list) into the table
int 	ext_table_new_prog(ext_table* c_table, const char *title);
// 		Insert a file extension into the linked list for specified program
int 	ext_table_ins_ext(ext_table* c_table, const char* title, const char* str);
// 		Search table for an extension. Return the program path associated with it.
char* 	ext_table_search_ext(ext_table* c_table, const char* search_term);
// 		Check if the specified program executes w/in current terminal or in background.
bool 	ext_table_prog_execs_in_term(ext_table* c_table, const char* prog_title);
// 		Clear table and free all allocated memory
int 	ext_table_free(ext_table* c_table);

#endif
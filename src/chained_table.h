#ifndef CHAINED_TABLE_H
#define CHAINED_TABLE_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>


typedef struct node_str {
	char *data;
	struct node_str* next;
} node_str;

typedef struct ll_str {
	struct node_str* head;
	char* title;
	bool exec_in_term;
} ll_str;

typedef struct chained_table_str {
	unsigned int size;
	unsigned int capacity;
	ll_str* list;
} chained_table_str;


int ct_str_init(chained_table_str* c_table);
int ct_str_grow(chained_table_str* c_table);
int ct_str_add_new_list(chained_table_str* c_table, const char *title);
int ct_str_ins_into_list(chained_table_str* c_table, const char* title, const char* str);
char* ct_str_search_table(chained_table_str* c_table, const char* search_term);
int ct_str_free_table(chained_table_str* c_table);
bool ct_str_exec_in_term(chained_table_str* c_table, const char* prog_title);

#endif
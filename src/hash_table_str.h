#ifndef HASH_TABLE_STR_H
#define HASH_TABLE_STR_H
#include "stdlib.h"
#include "string.h"

typedef struct node_str {
	char *data;
	struct node_str* next;
} node_str;

typedef struct ll_str {
	struct node_str* head;
} ll_str;

typedef struct hash_table_str {
	unsigned int size;
	ll_str* list;
	char** list_name;
} hash_table_str;

int init_hash_table_str(hash_table_str *ht, unsigned int m) 
{
	ht->size = m;
	ht->list = (ll_str*) malloc(sizeof(node_str) * m);
	for (int i = 0; i < m; i++) {
		ht->list[i].head = NULL;
	}
	ht->list_name = (char**) malloc(sizeof(char*) * m);
	return 0;
}

int grow_hash_table_str(hash_table_str *ht) 
{
	return 0;
}

int hash_func_str(hash_table_str *ht, const char *key)
{
	unsigned int sum = 0;
	for (int i = 0; i < strlen(key); i++) {
		unsigned int v = (unsigned int) *(key+i);
		sum += v * (i+1);
	}
	return sum % ht->size;
}


int insert_str(hash_table_str *ht, const char* key)
{
	node_str* new_node = (node_str*) malloc(sizeof(node_str));
	strcpy(new_node->data,key);
	new_node->next=NULL;

	unsigned int h = hash_func_str(ht, key);

	if (ht->list[h].head == NULL) {
		ht->list[h].head = new_node;
	}
	else {
		new_node->next = ht->list[h].head;
		ht->list[h].head = new_node;
	}
	return 0;
}

#endif 
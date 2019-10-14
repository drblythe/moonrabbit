#include "chained_table.h"

// Default table size is 10
int ct_str_init(chained_table_str* c_table)
{
	c_table->size = 0;
	c_table->capacity = 10;
	c_table->list = (ll_str*) malloc(sizeof(ll_str) * c_table->capacity);

	for (int i = 0; i < c_table->capacity; i++) {
		c_table->list->head = NULL;
		c_table->list->title = NULL;
	}
	return 0;
}

int ct_str_grow(chained_table_str* c_table)
{
	ll_str* new_list = (ll_str*) malloc(sizeof(ll_str) * (c_table->capacity * 2));
	for (int i = 0; i < c_table->capacity * 2; i++) {
		new_list[i].head = NULL;
		new_list[i].title = NULL;
	}
	for (int i = 0; i < c_table->size; i++) {
		new_list[i].title = c_table->list[i].title;
		new_list[i].head = c_table->list[i].head;
	}
	free(c_table->list);
	c_table->list = new_list;
	c_table->capacity *= 2;
}

int ct_str_add_new_list(chained_table_str* c_table, const char *title)
{
	for (int i = 0; i < c_table->size; i++) {
		if (!strcmp(title, c_table->list[i].title)) {
			return -1;
		}
	}

	if (c_table->size == c_table->capacity) {
		ct_str_grow(c_table);
	}

	c_table->list[c_table->size].title = (char*) malloc(sizeof(char) * (strlen(title)+1) );
	strcpy(c_table->list[c_table->size].title, title);
	c_table->list[c_table->size].head = NULL;
	c_table->size++;

	return 0;
}

int ct_str_ins_into_list(chained_table_str* c_table, const char* title, const char* str)
{

	for (int i = 0; i < c_table->size; i++) {
		if (!strcmp(c_table->list[i].title, title)) {
			node_str* new_node = (node_str*) malloc(sizeof(node_str));
			new_node->data = (char*) malloc(sizeof(char) * strlen(str));
			new_node->next = NULL;
			strcpy(new_node->data,str);

			if (c_table->list[i].head == NULL) {
				c_table->list[i].head = new_node;
			}
			else {
				new_node->next = c_table->list[i].head;
				c_table->list[i].head = new_node;
			}

			return 0;
		}
	}
	return -1;
}

char** ct_str_search_table(chained_table_str* c_table, const char* search_term)
{
	for (int i = 0; i < c_table->size; i++) {
		node_str *temp = c_table->list[i].head;
		while (temp != NULL) {
			if (!strcmp(temp->data, search_term)) {
				//int len = strlen(c_table->list[i].title);
				//char* new_str = malloc(sizeof(char)*(len+1));
				//strcpy(new_str,c_table->list[i].title);
				return &(c_table->list[i].title);
			}
			temp = temp->next;
		}
	}
	return (char**) NULL;
}

int ct_str_free_table(chained_table_str* c_table)
{
	return 0;
}
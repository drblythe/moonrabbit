#include "../include/ext_table.h"

// Default table size is 10
int ext_table_init(ext_table* c_table)
{
	c_table->size = 0;
	c_table->capacity = 10;
	c_table->list = (ext_linked_list*) malloc(sizeof(ext_linked_list) * c_table->capacity);

	for (int i = 0; i < c_table->capacity; i++) {
		c_table->list->head = NULL;
		c_table->list->title = NULL;
	}
	return 0;
}

int ext_table_free(ext_table* c_table)
{
	for (int i = 0; i < c_table->size; i++) {
		free(c_table->list[i].title);
		ext_node *tmp;
		while (c_table->list[i].head != NULL) {
			tmp = c_table->list[i].head;
			c_table->list[i].head = c_table->list[i].head->next;
			free(tmp->data);
			free(tmp);
		}
	}
	free(c_table->list);
	return 0;
}

int ext_table_grow(ext_table* c_table)
{
	ext_linked_list* new_list = (ext_linked_list*) malloc(sizeof(ext_linked_list) * (c_table->capacity * 2));
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
	return 0;
}

int	ext_table_new_prog(ext_table* c_table, const char *title)
{
	for (int i = 0; i < c_table->size; i++) {
		if (!strcmp(title, c_table->list[i].title)) {
			return -1;
		}
	}

	if (c_table->size == c_table->capacity) {
		ext_table_grow(c_table);
	}

	c_table->list[c_table->size].title = (char*) malloc(sizeof(char) * (strlen(title)+1) );
	strcpy(c_table->list[c_table->size].title, title);
	c_table->list[c_table->size].head = NULL;
	c_table->size++;

	return 0;
}

int ext_table_ins_ext(ext_table* c_table, const char* title, const char* str)
{

	for (int i = 0; i < c_table->size; i++) {
		if (!strcmp(c_table->list[i].title, title)) {
			ext_node* new_node = (ext_node*) malloc(sizeof(ext_node));
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

char* ext_table_search_ext(ext_table* c_table, const char* search_term)
{
	for (int i = 0; i < c_table->size; i++) {
		ext_node *temp = c_table->list[i].head;
		while (temp != NULL) {
			if (!strcmp(temp->data, search_term)) {
				return c_table->list[i].title;
			}
			temp = temp->next;
		}
	}
	return (char*) NULL;
}


bool ext_table_prog_execs_in_term(ext_table* c_table, const char* prog_title)
{
	for (int i = 0; i < c_table->size; i++) {
		if (!strcmp(prog_title, c_table->list[i].title)) {
			return c_table->list[i].exec_in_term;
		}
	}
	return 0;
}
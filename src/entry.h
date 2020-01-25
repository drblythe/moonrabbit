/* entry.h */
#ifndef ENTRY_H
#define ENTRY_H
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdbool.h>
#include <ncurses.h>
#include <limits.h> // limits.h defines: PATH_MAX = 4096, NAME_MAX = 255
#include "bindings.h"
#include "command_handling.h"
#include "utils.h"
#include "chained_table.h"

typedef struct ENTRY {
	char name[NAME_MAX];
	unsigned short index;
	char type;
	unsigned int size;
	char permission[10];
	gid_t gid;
	uid_t uid;
	bool marked;
} ENTRY;

typedef struct dir_index_pair {
	char *dir_name;
	int index;
} dir_index_pair;

typedef struct index_table {
	int capacity;
	int size;
	dir_index_pair* stored_index_array;
} index_table;

int index_table_init(index_table* table);
int index_table_store(index_table* table, const char* cwd, int current_index);
int index_table_load(index_table* table, const char* cwd, int* current_index);
int index_table_free(index_table*table);

int clear_entries(ENTRY* p_entry_arr, int* num_entries, int* current_index,int reset_index);
int get_entries(char* cwd, ENTRY** entry_arr, int* num_entries, int show_dots);
int display_entries(ENTRY* entry_arr,int num_entries, int current_index,int LINES);
int display_file_info(char* cwd, ENTRY entry,int current_index, int num_entries);

int mark_file(ENTRY *p_entry, int *num_selected);
int unmark_file(ENTRY *p_entry, int *num_selected);

int update_curr_index(short int direction, int* current_index, int *num_entries);

int search_dir(const char* file_name, ENTRY* entry_arr, int* current_index, const int num_entries);
int binarysearch_dir(const char* file_name, ENTRY* arr, const int num_entries, int* current_index);

int get_num_marked(int num_entries, ENTRY* entry_arr);
int create_copy_buffer(char*** copy_buff, int buff_size);
int fill_copy_buffer(char*** copy_buff, int buff_size, int num_entries, ENTRY* entry_arr, char* cwd);
int empty_copy_buffer(char*** copy_buff, int* buff_size);

#endif
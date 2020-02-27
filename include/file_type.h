/* file_type.h */
#ifndef FILE_TYPE_H
#define FILE_TYPE_H
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
#include "cmd_handling.h"
#include "utils.h"
#include "ext_table.h"

typedef struct file_t {
	char name[NAME_MAX];
	unsigned short index;
	char type;
	unsigned int size;
	char permission[10];
	gid_t gid;
	uid_t uid;
	bool marked;
} file_t;

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
int store_index(index_table* table, const char* cwd, int current_index);
int load_index(index_table* table, const char* cwd, int* current_index);
int index_table_free(index_table*table);

int clear_file_list(file_t* p_file_arr, int* num_entries, int* current_index,int reset_index);
int get_file_list(char* cwd, file_t** file_arr, int* num_entries, int show_dots);

int mark_file(file_t *p_file, int *num_selected);
int unmark_file(file_t *p_file, int *num_selected);

int update_curr_index(short int direction, int* current_index, int *num_entries);

int search_dir(const char* file_name, file_t* file_arr, int* current_index, const int num_entries);
int binarysearch_dir(const char* file_name, file_t* arr, const int num_entries, int* current_index);

int get_num_marked(int num_entries, file_t* file_arr);
int create_copy_buffer(char*** copy_buff, int buff_size);
int fill_copy_buffer(char*** copy_buff, int buff_size, int num_entries, file_t* file_arr, char* cwd);
int empty_copy_buffer(char*** copy_buff, int* buff_size);

#endif
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
#include <limits.h> /* 	limits.h defines: PATH_MAX = 4096, NAME_MAX = 255 */
#include "bindings.h"
#include "command_handling.h"
#include "utils.h"
#define MAX_STORED_INDEXES 20

typedef struct ENTRY {
	char name[NAME_MAX];
	unsigned short index;
	char type;
	unsigned int size;
	char permission[10];
	gid_t gid;
	uid_t uid;
	unsigned short marked; // change to bool or bit stuff!
	//char owner[255];
} ENTRY;

typedef struct KEY_VALUE {
	char key[NAME_MAX];
	unsigned int value;
} KEY_VALUE;

int clear_entries(ENTRY* p_entry_arr, int* num_entries, int* current_index,int reset_index);
int get_entries(char* cwd, ENTRY** entry_arr, int* num_entries, int show_dots);
int display_entries(ENTRY* entry_arr,int num_entries, int current_index,int LINES);
int display_file_info(char* cwd, ENTRY entry,int current_index, int num_entries);

int mark_file(ENTRY *p_entry, int *num_selected);
int unmark_file(ENTRY *p_entry, int *num_selected);

int update_curr_index(short int direction, int* current_index, int *num_entries);
int save_index(KEY_VALUE** p_kvp_arr, int num_stored, const char* dir_path, const int index);
int load_index(KEY_VALUE** p_kvp_arr, const int num_stored, const char* cwd, int* current_index);

int search_dir(const char* file_name, ENTRY* entry_arr, int* current_index, const int num_entries);
int binarysearch_entry(const char* file_name, ENTRY* arr, const int num_entries, int* current_index);

int get_num_marked(int num_entries, ENTRY* entry_arr);
int create_copy_buffer(char*** copy_buff, int buff_size);
int fill_copy_buffer(char*** copy_buff, int buff_size, int num_entries, ENTRY* entry_arr, char* cwd);
int empty_copy_buffer(char*** copy_buff, int* buff_size);
int print(char*** copy_buff, int buff_size);


#endif
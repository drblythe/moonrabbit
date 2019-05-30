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
#define MAX_STORED_INDEXES 20

typedef struct ENTRY {
	char name[NAME_MAX];
	unsigned short index;
	char type;
	unsigned int size;
	char permission[10];
	gid_t gid;
	uid_t uid;
	unsigned short marked;
	//char owner[255];
} ENTRY;

typedef struct KEY_VALUE {
	char key[NAME_MAX];
	unsigned int value;
} KEY_VALUE;

int init_ncurses(WINDOW *win);

int clear_entries(ENTRY* p_entry_arr, int* num_entries, int* current_index,int reset_index);
int get_entries(char* cwd, ENTRY** entry_arr, int* num_entries, int show_dots);
int display_entries(ENTRY* entry_arr,int num_entries, int current_index,int LINES);
int display_file_info(char* cwd,ENTRY entry,int current_index, int num_entries);

int is_directory(char* cwd, char* name);
int mark_file(ENTRY *p_entry);
int unmark_file(ENTRY *p_entry);
char* get_abs_path(char* cwd, char* file_name);
char* get_permissions(char* cwd, char* file_name);

int update_curr_index(short int direction, int* current_index, int *num_entries);
int save_index(KEY_VALUE** p_kvp_arr, int* p_num_stored, const char* dir_path, const int index);
int restore_index(KEY_VALUE** p_kvp_arr, int* p_num_stored, char** p_cwd, int* current_index);
#endif

/* entry.h */

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
#include <limits.h> /* 	limits.h defines:
						PATH_MAX = 4096
						NAME_MAX = 255 */

#define UP -1
#define DOWN 1

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


int init_ncurses(WINDOW *win);

int update_curr_index(short int direction, int* current_index, int *num_entries);

int mark_file();

int get_file_info(char* cwd, char* name);

char* get_abs_path(char* cwd, char* file_name);

int is_directory(char* cwd, char* name);

int get_entries(char* cwd, ENTRY** entry_arr, int* num_entries, int show_dots);

int display_entries(ENTRY* entry_arr,int num_entries, int current_index,int LINES);

int clear_entries(ENTRY* p_entry_arr, int* num_entries, int* current_index);

int init_ncurses(WINDOW *win);

int update_curr_index(short int direction, int* current_index, int *num_entries);

int mark_file(ENTRY *p_entry);

int unmark_file(ENTRY *p_entry);

int display_file_info(char* cwd,ENTRY entry,int current_index, int num_entries);

char* get_permissions(char* cwd, char* file_name);

int check_permissions(char* action, char* cwd, ENTRY entry);

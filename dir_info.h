/* dir_info.h */
#ifndef DIR_INFO_H
#define DIR_INFO_H
#include <limits.h>
#define MAX_DIRS_STORED 10

typedef struct MUH_DIR_TYPE {
//	static unsigned short count;
	unsigned int active_index;
	char path[256];
	//char path[NAME_MAX];
} MUH_DIR_TYPE;

MUH_DIR_TYPE dir_indexes[MAX_DIRS_STORED];
int check_size();
int store_index(int current_index, char *cwd);
int load_index(int *current_index, char *cwd/*, int num_entries*/);
#endif

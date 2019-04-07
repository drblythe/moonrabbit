/* dir_info.h */
#include <limits.h>

typedef struct U_DIR {
	unsigned int active_index;
	char path[NAME_MAX];
} U_DIR;

int store_index(U_DIR *direc, int *dirs_stored, int current_index, char *cwd);

int load_index(U_DIR *direc, int *current_index, char *cwd);


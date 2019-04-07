/* dir_info.h */
#include <limits.h>

typedef struct U_DIR {
	unsigned int active_index;
	char path[NAME_MAX];
} U_DIR;

int add_direc(U_DIR *direc_array);

int store_index(int *current_index, char *cwd);

int load_index(int *current_index, char *cwd);


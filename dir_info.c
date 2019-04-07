/* dir_info.c */
#include "dir_info.h"

int add_direc(U_DIR *direc_array)
{
	return 1;
}

int store_index(U_DIR *direc, int current_index, char *cwd)
{
	direc->active_index = current_index;
	return 1;
}

int load_index(U_DIR *direc, int *current_index, char *cwd)
{
	*current_index = direc->active_index;
	return 1;
}


/* dir_info.c */
#include "dir_info.h"

int store_index(U_DIR *direc, int *dirs_stored, int current_index, char *cwd)
{
	// if directory name already in array, just update the active_index
	for (int i = 0; i < *dirs_stored; i++) {
		if ( (*(direc+i)).path == cwd ) {
			(*(direc+i)).active_index = current_index;
			return 1;
		}
	}
	// if it is not, set *(direc+ (*dirs_stored) ).name = cwd and active index
	// 	to be current_index ... increment *dirs_stored after
	(*(direc+(*dirs_stored))).active_index = current_index;
	*dirs_stored++;
	// in main or something, another function, check if dirs stored is
	// 	greater than whatever i decide max is, if it is  remove first item,
	// 	move everything else back by one
	return 1;
}

int load_index(U_DIR *direc, int *current_index, char *cwd)
{
	*current_index = direc->active_index;
	return 1;
}


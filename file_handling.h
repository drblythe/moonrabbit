/*file_handling.h*/
#ifndef	FILE_HANDLING_H
#define FILE_HANDLING_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
/*
char *EDITOR; 	// defaults[0]
char *AUDIO; 	// defaults[1]
char *VIDEO; 	// defaults[2]
char *IMAGES; 	// defaults[3]
char *PDF; 		// defaults[4]
*/
char	*DEFAULTS[5];
int 	set_default_programs(char* config_path, char **DEFAULTS);
int 	prev_dir(char** p_cwd);
int 	next_dir(char** p_cwd, char* dir_name);
int 	open_file(char* type, char* cwd, char* file_name);
#endif

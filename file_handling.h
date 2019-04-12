/*file_handling.h*/
#ifndef	FILE_HANDLING_H
#define FILE_HANDLING_H
#define TEXT 0
#define AUDIO 1
#define VIDEO 2
#define IMAGES 3
#define PDF 4
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
/*
char *TEXT; 	// defaults[0]
char *AUDIO; 	// defaults[1]
char *VIDEO; 	// defaults[2]
char *IMAGES; 	// defaults[3]
char *PDF; 		// defaults[4]
*/
char	*DEFAULTS[5];
int 	set_default_programs(char* config_path);
int 	prev_dir(char** p_cwd);
int 	next_dir(char** p_cwd, char* dir_name);
int 	open_file(char* cwd, char* file_name);
#endif

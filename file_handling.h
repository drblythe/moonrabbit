/*file_handling.h*/
#ifndef	FILE_HANDLING_H
#define FILE_HANDLING_H
/*
#define TEXT 0
#define AUDIO 1
#define VIDEO 2
#define IMAGE 3
#define DOC 4
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

char TEXT[64]; 
char AUDIO[64]; 
char VIDEO[64]; 
char IMAGE[64]; 
char DOC[64]; 
int 	set_default_programs(char* config_path);
int 	prev_dir(char **p_cwd);
int 	next_dir(char **p_cwd, char *dir_name);
int 	open_file(char *cwd, char *file_name);
char*	get_extension(char *file_name);
char 	get_file_type(char *file_name);
#endif

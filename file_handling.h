/*file_handling.h*/
#ifndef	FILE_HANDLING_H
#define FILE_HANDLING_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

int set_default_programs(char* config_path, char* TEXT, char* AUDIO, char* VIDEO, char* IMAGE, char* DOC, char* SHELL, char* TERMINAL);

int prev_dir(char **p_cwd);

int next_dir(char **p_cwd, char *dir_name);

int open_file(char *cwd, char *file_name, char* TEXT, char* AUDIO, char* VIDEO, char* IMAGE, char* DOC, char* SHELL, char* TERMINAL);

char* get_extension(char *file_name);

char get_file_type(char *file_name);

#endif

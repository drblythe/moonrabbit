#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>



int prev_dir(char** p_cwd);

int next_dir(char** p_cwd, char* dir_name);

int open_file(char* cwd, char* file_name);

int change_dir(char** p_cwd, char* dir_name);

int make_dir(char* cwd, char* dir_name);

int select_file(char* cwd, char* file_name);

char* get_abs_path(char* cwd, char* file_name);
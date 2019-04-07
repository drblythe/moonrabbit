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

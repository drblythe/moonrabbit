#ifndef UTILS_H
#define UTILS_H
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>

int is_directory(char* cwd, char* name);
int is_directory_fullpath(char* path);
char* get_permissions(char* cwd, char* file_name);
char** tokenize_command(char* command);
int str_remove_outer_ws(char* str);
char* str_concat_cwd_filename(const char* cwd, const char* filename);
#endif

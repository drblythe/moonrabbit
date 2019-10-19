#ifndef UTILS_H
#define UTILS_H
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <pwd.h>

int is_directory(char* cwd, char* name);
int is_directory_fullpath(char* path);
char* get_permissions(const char* cwd, const char* file_name);
bool open_dir_allowed(const char* cwd, const char* file_name);
char** tokenize_command(char* command);
int str_remove_outer_ws(char* str);
char* str_concat_cwd_filename(const char* cwd, const char* filename);
#endif

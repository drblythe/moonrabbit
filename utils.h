#ifndef UTILS_H
#define UTILS_H
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
int is_directory(char* cwd, char* name);
int is_directory_fullpath(char* path);
char* get_permissions(char* cwd, char* file_name);
#endif

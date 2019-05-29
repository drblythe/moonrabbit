#ifndef COMMAND_HANDLING_H
#define COMMAND_HANDLING_H
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int change_dir(char** p_cwd, char* dir_name);
int handle_cmd(const char *input, char** p_cwd);
int copy(const char *dest, const char *src);
/* ^^ https://stackoverflow.com/questions/2180079/how-can-i-copy-a-file-on-unix-using-c  */
int delete_file(const char * file_path);
int delete_dir(const char * dir_path, const int num_subitems);
int resolve_path(char * dir_path, char *file_name);
#endif

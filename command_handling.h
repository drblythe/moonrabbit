#ifndef COMMAND_HANDLING_H
#define COMMAND_HANDLING_H
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>
#include <stdio.h>

int cmd_cd(char** p_cwd, char* dir_name);
int handle_cmd(char **p_input, char** p_cwd);
int cmd_copy(const char *dest, const char *src);
/* ^^ https://stackoverflow.com/questions/2180079/how-can-i-copy-a-file-on-unix-using-c  */
int cmd_delete(const char * file_path);
int resolve_path(char * dir_path, char *file_name);
//char** tokenize_command(char* command, char** p_command);
char** tokenize_command(char* command);
#endif

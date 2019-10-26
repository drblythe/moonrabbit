#ifndef COMMAND_HANDLING_H
#define COMMAND_HANDLING_H
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <assert.h>
#include <stdio.h>
#include <ftw.h>
#include "utils.h"

int cmd_cd(char** p_cwd, char* dir_name);
int handle_cmd(char **p_input, char** p_cwd);
int cmd_copy(const char *dest, const char *src);
int cmd_copy_dir(int del_after_move, char* dest_dir, char* src_dir);
int cmd_delete(const char * file_path);
int cmd_delete_dir(const char* dir_path);
int cmd_move(const char* dest, const char* src);
int cmd_mkdir(char* path);
int cmd_mkdir_splitpath(char* path, char* file_name);
#endif

#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
int change_dir(char** p_cwd, char* dir_name);
int delete_file(char *cwd, char *file_name);
//int open_shell(char *cwd);
int handle_cmd(char *input,char **p_cwd);
int find_in_dir(char *name);
int move_to_entry(char *name);

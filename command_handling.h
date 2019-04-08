#include <string.h>
#include <stdlib.h>

int change_dir(char** p_cwd, char* dir_name);

int make_dir(char* cwd, char* dir_name);

int delete_file(char *cwd, char *file_name);

int open_shell(char *cwd);

int handle_cmd(char *input,char **p_cwd);

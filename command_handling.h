#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
int change_dir(char** p_cwd, char* dir_name);
int handle_cmd(char *input,char **p_cwd);

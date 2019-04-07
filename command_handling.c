#include "command_handling.h"

int change_dir(char** p_cwd, char* dir_name)
{
    strcat(*p_cwd, "/");
    strcat(*p_cwd, dir_name);
    return 1;
}

int make_dir(char* cwd, char* dir_name)
{
    return 1;
}

int delete_file(char *cwd, char *file_name)
{
    return 1;
}

int handle_cmd()
{
/* 
 * separate command and arguments
 * put args into vector
 * depending on the command call appropriate function from above
 */
	return 1;
}

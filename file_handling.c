#include "file_handling.h"


int prev_dir(char** p_cwd)
/*int prev_dir(&cwd)*/
{
	strcat(*p_cwd, "/..");
	strcpy(*p_cwd,realpath(*p_cwd,NULL));
	return 1;
}


int next_dir(char** p_cwd, char* dir_name)
{
	if (strcmp(*p_cwd,"/"))
		strcat(*p_cwd, "/");
	strcat(*p_cwd, dir_name);
	return 1;
}


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


int open_file(char* cwd, char* file_name)
{
    // fork() returns child's pid
    pid_t pid;
    pid = fork();
    if (pid == -1)
        perror("fork");

    if (!pid) {
        char *args[] = {"/usr/bin/urxvt", "-e", "vim $HOME/.Xresources", NULL};

        int ret;

        ret = execv(args[0], args);

        if (ret == -1) {
            perror("execv");
            exit (EXIT_FAILURE);
        }
    }

    //exit(EXIT_SUCCESS);
	
	return 1;
}

int accept_input()
{
	
	return 1;
}


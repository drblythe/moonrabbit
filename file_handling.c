#	include "file_handling.h"


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
	int ret;
   // pid_t pid;
    //pid = fork();
	
    //if (pid == -1)
     //   perror("fork");

	  //  if (!pid) {
     		char path[strlen(cwd) + 1 + strlen(file_name) + 1];
			strcpy(path,cwd);
			strcat(path, "/");
			strcat(path, file_name);

			char command[3 + 1 +strlen(path) + 1];
			strcpy(command, "vim");
			strcat(command, " ");
			strcat(command, path);
			strcat(command, " 2>/dev/null");
	ret = system(command);

   	//		char *args[] = {"/usr/bin/urxvt", "-e", "bash", "-i", "-c", command, NULL};


   			//ret = execv(args[0], args);

     //   if (ret == -1) {
      //      perror("execv");
            //exit (EXIT_FAILURE);
   //     }
  //  }

	
	return 1;
}

int accept_input()
{
	
	return 1;
}


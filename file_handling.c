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



int open_file(char* type, char* cwd, char* file_name)
{
	int ret;
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

	return 1;
}

int set_default_programs(char* config_path, char** DEFAULTS)
{
    FILE *fp;
	int n;
    char line[255];
	char prog[64];
    fp = fopen(config_path,"r");
	n = 0;
	while (fgets(line,255,(FILE*)fp))
        if (line[0] == '#')
            continue;
        else {
			while (line[n] != ' ') {
				prog[n] = line[n];
				n++;
			}
			prog[n] = '\0';
			strcpy(DEFAULTS[def_index], prog);
		}
		if (!strcmp(prog,"EDITOR"))
		for (int i = 0; i < n; i++)
			prog[i] = '\0';
		n = 0;
    fclose(fp);

    return 1;
}







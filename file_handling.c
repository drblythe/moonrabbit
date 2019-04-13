/* file_handling.c */
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


int open_file(char* cwd, char* file_name)
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

int set_default_programs(char* config_path)
// DEFAULTS is the array of default programs that are passed in
{
	//init, open correct file
	FILE *fp;
	int n;
	int m;
	char cfg_line[255];
	char filetype[64];
	char program[64];
	fp = fopen(config_path,"r");
	n = 0;
	m = 0;

	// while there are still lines to read, parse the config file
	// parsing
	// 		lines commented by # -- immediately move to next loop if 0 char is #
	// 		get index of first ' '
	// 			0 up to index is filetype
	// 			index up to next space is program
	// 		copy those chars into filetype and program, terminate with '\0'
	
	while (fgets(cfg_line,255,(FILE*)fp)) {
		n = 0;
		m = 0;
		if (cfg_line[0] == '#' || cfg_line[0] == '\n')
			continue;
		else {
			while(cfg_line[n] != '='){
				filetype[n] = cfg_line[n];
				n++;
			}
			filetype[n] = '\0';
			while(cfg_line[n] != '\0') {
				program[m] =  cfg_line[n+1];
				n++;
				m++;
			}
			program[m] = '\0';
		// check file type, set appropriate char array
		if (!strcmp(filetype,"TEXT")) 
			strcpy(TEXT,program);
		else if (!strcmp(filetype,"AUDIO"))
			strcpy(AUDIO,program);
		else if (!strcmp(filetype,"VIDEO"))
			strcpy(VIDEO,program);
		else if (!strcmp(filetype,"IMAGE"))
			strcpy(IMAGE,program);
		else if (!strcmp(filetype,"DOC"))
			strcpy(DOC,program);
		}
	}

	fclose(fp);
    return 1;
}
















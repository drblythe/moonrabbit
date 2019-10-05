#include "utils.h"

int is_directory(char* cwd, char* name)
{

	struct stat sb;
	int ret;
	char abs_path[NAME_MAX];
	strcpy(abs_path,cwd);
	strcat(abs_path,"/");
	strcat(abs_path,name);
	(stat(abs_path, &sb) == 0 && S_ISDIR(sb.st_mode)) ? (ret=1) : (ret=0);
	return ret;
}

int is_directory_fullpath(char* path)
{
	struct stat sb;
	int ret;
	(stat(path, &sb) == 0 && S_ISDIR(sb.st_mode)) ? (ret=1) : (ret=0);
	return ret;
}

char* get_permissions(char* cwd, char* file_name)
{
	int len_cwd = strlen(cwd);
	int len_name = strlen(file_name);
	int len_file_path = len_cwd + len_name + 1;

	char file_path[len_file_path];
	strcpy(file_path, cwd);
	strcat(file_path,"/");
	strcat(file_path,file_name);

	struct stat st;
	char *modeval = malloc(sizeof(char)*9 + 1);
	if (stat(file_path, &st) == 0) {
        mode_t perm = st.st_mode;
		modeval[0] = (perm & S_IRUSR) ? 'r' : '-';
		modeval[1] = (perm & S_IWUSR) ? 'w' : '-';
	    modeval[2] = (perm & S_IXUSR) ? 'x' : '-';
	    modeval[3] = (perm & S_IRGRP) ? 'r' : '-';
	    modeval[4] = (perm & S_IWGRP) ? 'w' : '-';
	    modeval[5] = (perm & S_IXGRP) ? 'x' : '-';
	    modeval[6] = (perm & S_IROTH) ? 'r' : '-';
	    modeval[7] = (perm & S_IWOTH) ? 'w' : '-';
	    modeval[8] = (perm & S_IXOTH) ? 'x' : '-';
	}
	else{
	    //return strerror(errno);
	    for (int i = 0; i < 9; i++) {
	    	modeval[i] = '-';
	    }
	}
	modeval[9] = '\0';

	return modeval;
}




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

bool open_dir_allowed(const char* cwd, const char* file_name)
{
	bool allowed = 0;
	char* perm;
	uid_t uid = getuid();
	gid_t gid = getgid();
	char* fullpath = str_concat_cwd_filename(cwd, file_name);

	if (uid == 0){
		allowed = 1;
	}
	else {
		struct stat sb;
		int ret = stat(fullpath, &sb);
		if (ret == -1) return 0;

		perm = get_permissions(cwd, file_name);
		if (perm[8] == 'x') {
			allowed = 1;
		}
		else if (sb.st_uid == uid && perm[2] == 'x'){
			allowed = 1;
		}
		else if (sb.st_gid == gid && perm[5] == 'x'){
			allowed = 1;
		}
	}

	free(fullpath);
	free(perm);
	return allowed;
}

char* get_permissions(const char* cwd, const char* file_name)
{
	int len_cwd = strlen(cwd);
	int len_name = strlen(file_name);
	int len_file_path = len_cwd + 1 + len_name + 1;

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

char** tokenize_command(char* command) 
{
	char a_delim = ' ';
	char** result    = 0;
    size_t count     = 0;
    char* tmp        = command;
    char* last_space = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_space = tmp;
        }
        tmp++;
    }
    count += last_space < (command + strlen(command) - 1);
	count++;
    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(command, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }
    return result;
}

int str_remove_outer_ws(char* str)
{
	int len = strlen(str);
	int m = len - 1;
	int n = 0;
	while (str[m] == ' ') {
		str[m] = '\0';
		m--;
	}
	while (str[n] == ' ') {
		n++;
	}
	for (int i = n; i < len; i++) {
		str[i-n] = str[i];
	}
	return 0;
}

char* str_concat_cwd_filename(const char* cwd, const char* filename)
{
	unsigned int len = strlen(cwd) + 1 + strlen(filename) + 1;
	char* full_path = malloc(sizeof(char) * len);
	strcpy(full_path,cwd);
	strcat(full_path,"/");
	strcat(full_path,filename);
	return full_path;
}
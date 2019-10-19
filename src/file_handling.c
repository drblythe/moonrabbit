/* file_handling.c */
#include "file_handling.h"

int prev_dir(char** p_cwd)
{
	strcat(*p_cwd, "/..");
	strcpy(*p_cwd,realpath(*p_cwd,NULL));
	return 1;
}

int next_dir(char** p_cwd, char* dir_name)
{
	if (!open_dir_allowed(*p_cwd,dir_name)) {
		//print warning
		return 0;
	}
	if (strcmp(*p_cwd,"/")){
		strcat(*p_cwd,"/");
	}
	strcat(*p_cwd, dir_name);
	return 1;
}

char* get_extension(char* file_name)
{
	int n;
	char *ext;
	n = 0;

	for (int i = strlen(file_name)-1; i >= 0; i--) {
		if (file_name[i] == '.') {
			ext = malloc(sizeof(char)*(strlen(file_name)-i));
			for(int j = i; j < strlen(file_name); j++){
				*(ext+n) = file_name[j];
				n++;
			}
			*(ext+(strlen(file_name)-i)) = '\0';
			return ext;
		}
	}

	// If there is no extension, set to ".<none>"
	ext = malloc(sizeof(char)*(7+1));
	*(ext+0) = '.';
	*(ext+1) = '<';
	*(ext+2) = 'n';
	*(ext+3) = 'o';
	*(ext+4) = 'n';
	*(ext+5) = 'e';
	*(ext+6) = '>';
	*(ext+7) = '\0';
	return ext;
}

int open_file(char *cwd, char *file_name, chained_table_str* ct)
{
	bool exec_in_term = 0;
	char file_type;	
	int ret;
	char *ext = get_extension(file_name);
	char *program_path = ct_str_search_table(ct, ext);
	if (program_path == NULL) {
		return -1;
	}
	char * full_filepath = str_concat_cwd_filename(cwd, file_name);
	exec_in_term = ct_str_exec_in_term(ct,program_path);
	int status;


	pid_t pid = fork();
	if (pid < 0) {
		perror("Could not fork process.");
		exit(1);
	}
	else if (pid == 0) {
		if (!exec_in_term) {
			int fd = open("/dev/null", O_WRONLY);
			dup2(fd, 1);
			dup2(fd, 2);
		}
		char* args[] = {
			program_path,
			full_filepath,
			NULL
		};
		execv(args[0],args);
	}
	if (exec_in_term) {
		waitpid(pid, &status, 0);
	}
	else {
		waitpid(pid, &status, WNOHANG);
	}
	
	free(ext);
	free(full_filepath);
	return 1;
}

int file_name_len(char* path)
{
	int len = 0;
	for(int i = strlen(path)-1; i >= 0; i--) {
		if (path[i] == '/') 
			break;
		len++;
	}
	return len;
}

int extract_file_name(char** buff, const char* path)
{
	int len = 0;
	int slash_index = 0;
	for(int i = strlen(path)-1; i >= 0; i--) {
		if (path[i] == '/') {
			slash_index = i;
			break;
		}
		len++;
	}
	*buff = malloc(sizeof(char)*(len+1));
	for (int i = 0; i < len; i++) {
		*(*buff+i) = path[slash_index+1+i];
	}
	*(*buff+len) = '\0';

	return len;
}

int append_to_path(char* new_path, const char* old_path, const char* filename) 
{
	strcpy(new_path, old_path);
	strcat(new_path, "/");
	strcat(new_path,filename);
	return 1;
}


int file_buff_cp(int del_after_copy, const char* dest, char** file_buffer, int* size)
{
	for (int i = 0; i < *size; i++) {
		char *filename;
		char ** p = &filename;
		int len = 0;
		//extract file name
		len = extract_file_name(p,*(file_buffer+i));
		//append filename to new path
		char new_path[strlen(dest)+1+len+1];
		append_to_path(new_path, dest, filename);

		if (is_directory_fullpath(*(file_buffer+i))) {
			cmd_copy_dir(del_after_copy, new_path, *(file_buffer+i));
		}
		else {
			//move files if del_after_copy
			if (del_after_copy)
				cmd_move(new_path, *(file_buffer+i));
			else
				cmd_copy(new_path, *(file_buffer+i));
			//copy otherwise
		}
		free(*p);
	}
	return 1;
}

int delete_selection(char** file_buffer, int* num_selected)
{
	for (int i = 0; i < *num_selected; i++) {
		if (is_directory_fullpath(*(file_buffer+i))) {
			cmd_delete_dir(*(file_buffer+i));
		}
		else {
			cmd_delete(*(file_buffer+i));
		}
	}
	*num_selected = 0;
	return 1;
}

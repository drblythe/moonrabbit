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
	// the char pointer size will be malloc'ed when the size of the extension is known
	//
	// taking dot into consideration, move an addition index forward when starting 
	// the copy
	n = 0;
/*
 * first return statement is returning a null pointer/value
 * something wrong with iteration	
 */
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
	// if it reaches the end (no period) assume to open with TEXT editor
	// so set extension to "unknown" or something
	ext = malloc(sizeof(char)*(7+1));
	*(ext+0) = 'u';
	*(ext+1) = 'n';
	*(ext+2) = 'k';
	*(ext+3) = 'n';
	*(ext+4) = 'o';
	*(ext+5) = 'w';
	*(ext+6) = 'n';
	*(ext+7) = '\0';
	return ext;
}


char get_file_type(char* file_name)
{
	// call get_extension, store in variable
	char file_type;
	char * ext;
	ext = get_extension(file_name);
	// check extension against those given in config, or against known types
	// do it later, hard code nao
	
	// audio
	if (!strcmp(ext,".mp3") || !strcmp(ext,".flac")||
		!strcmp(ext,".m4a"))
		file_type = 'A';
	// check audio extension array from config if nothing here

	// video
	else if (!strcmp(ext,".mkv") || !strcmp(ext,".mp4")||
		!strcmp(ext,".avi") || !strcmp(ext,".gif"))
		file_type = 'V';
	// check video extension array from config if nothing here 

	// image
	else if (!strcmp(ext,".jpg") || !strcmp(ext,".jpeg")||
			!strcmp(ext,".JPG") || !strcmp(ext,".JPEG")||
			!strcmp(ext,".png") || !strcmp(ext,".PNG")) 
		file_type = 'I';
	// check image extension array from config if nothing here 

	// document
	else if (!strcmp(ext,".pdf") ||
		!strcmp(ext,".djvu"))
		file_type = 'D';
	// check docs extension array from config if nothing here

	else
		file_type = '0';
	
	// free mem at ext when done
	free(ext);
	return file_type;
}

int open_file(char *cwd, char *file_name, char* TEXT, char* AUDIO, 
	char* VIDEO, char* IMAGE, char* DOC, char* SHELL, char* TERMINAL)
{
	int ret;
	char file_type;	

	int send_to_bg = 1;

/* Change this bullshit below ... should be a command 
 * char array based on strlen(program) 
 */
	char * command = malloc(sizeof(char)*512);

	file_type = get_file_type(file_name);
	switch(file_type){
	case 'A':
		strcpy(command, AUDIO);
		break;
	case 'V':
		strcpy(command, VIDEO);
		break;
	case 'I':
		strcpy(command, IMAGE);
		break;
	case 'D':
		strcpy(command, DOC);
		break;
	default:
		send_to_bg = 0;
		strcpy(command,TEXT);
		break;
	}
	int need_quotes = 0;
	for (int i = 0; i < strlen(cwd); i++) {
		if (cwd[i] == ' ' || cwd[i] == '-')
			need_quotes = 1;
	}

	strcat(command, " ");
	if (need_quotes)
		strcat(command,"\"");
	strcat(command, cwd);
	strcat(command, "/");
	if (!need_quotes)
		strcat(command,"\"");
	strcat(command, file_name);
		strcat(command,"\"");
	if (send_to_bg)
		strcat(command, " &");
	ret = system(command);
	if (ret) {
		perror("system");
		return -1;
	}
	free(command);
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
		//printf("--%c--\n",*(*buff+i));
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















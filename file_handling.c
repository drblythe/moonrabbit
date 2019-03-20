#include "file_handling.h"



int prev_dir(char** p_cwd, int* current_index, int* prev_index, int* next_index)
/*int prev_dir(&cwd)*/
{
	*next_index = *current_index;
	*current_index = *prev_index;
	strcat(*p_cwd, "/..");
	strcpy(*p_cwd,realpath(*p_cwd,NULL));
	return 1;
}


int next_dir(char** p_cwd, char* dir_name, int* current_index, int* prev_index, int* next_index)
{
	*prev_index = *current_index;
	*current_index = *next_index;
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

	return 1;
}


int select_file(char* cwd, char* file_name)
{
	// set the mark ENTRY attribute
	return 1;
}






char* get_abs_path(char* cwd, char* file_name)
{
	int len_cwd = strlen(cwd);
	int len_file_name = strlen(file_name);
	int len_abs_path = len_cwd + len_file_name + 1;

	char* abs_path = malloc(sizeof(char)*len_abs_path);
	strcpy(abs_path,cwd);
	strcat(abs_path,"/");
	strcat(abs_path,file_name);

	return abs_path;
}


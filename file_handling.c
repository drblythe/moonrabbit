/* file_handling.c */
#include "file_handling.h"
#include <stdio.h>
#include <ctype.h>

int prev_dir(char** p_cwd)
{
	strcat(*p_cwd, "/..");
	strcpy(*p_cwd,realpath(*p_cwd,NULL));
	return 1;
}


int next_dir(char** p_cwd, char* dir_name)
{
	if (strcmp(*p_cwd,"/")){
		strcat(*p_cwd, "/");
	}
	strcat(*p_cwd, dir_name);
	
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
		// get ridda that dang ole newline that bugged mah sheeit
		for (int i = 0; i < strlen(program); i++)
			if (program[i] == '\n')
				program[i] = '\0';
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
		!strcmp(ext,".avi"))
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

int open_file(char* cwd, char* file_name)
{
	int ret;
	char file_type;	

	int send_to_bg = 1;

/* Change this bullshit below ... should be a command char array based on strlen(program) */
	char * command = NULL;
	command = malloc(sizeof(char)*512);
	//char program[32];

	file_type = get_file_type(file_name);
	switch(file_type){
	case 'A':
		strcat(command, AUDIO);
		break;
	case 'V':
		strcat(command, VIDEO);
		break;
	case 'I':
		strcat(command, IMAGE);
		break;
	case 'D':
		strcat(command, DOC);
		break;
	default:
		send_to_bg = 0;
		strcat(command,TEXT);
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
	free(command);
	
	return 1;
}






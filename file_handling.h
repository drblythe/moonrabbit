/*file_handling.h*/
#ifndef	FILE_HANDLING_H
#define FILE_HANDLING_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
char TEXT[64]; 
char AUDIO[64]; 
char VIDEO[64]; 
char IMAGE[64]; 
char DOC[64]; 
char SHELL[64];
char TERMINAL[64];
//char TEXT_EXT[256];
//char AUDIO_EXT[256];
//char VIDIO_EXT[256];
//char IMAGE_EXT[256];
//char DOC_EXT[256];
int 	set_default_programs(char* config_path);
int 	prev_dir(char **p_cwd);
int 	next_dir(char **p_cwd, char *dir_name);
int 	open_file(char *cwd, char *file_name);
int		open_shell(char *cwd);
char* 	get_extension(char *file_name);
char 	get_file_type(char *file_name);
#endif

#ifndef INIT_H
#define INIT_H
#include <ncurses.h>
#include <limits.h>
#include "utils.h"
#include "chained_table.h"

#define MAX_PROG_PATH_LEN 64

char TEXT[PATH_MAX]; 
char AUDIO[PATH_MAX]; 
char VIDEO[PATH_MAX]; 
char IMAGE[PATH_MAX]; 
char DOC[PATH_MAX]; 
char SHELL[PATH_MAX];
char TERMINAL[PATH_MAX];

char** AUDIO_EXTENSIONS;
char** VIDEO_EXTENSIONS;
char** IMAGE_EXTENSIONS;
char** DOC_EXTENSIONS;

chained_table_str ct;


int init_ncurses(WINDOW *win)
{
	setlocale(LC_ALL, ""); // Set locale to be able to properly display unicode. Must precede initscr()
	initscr();
	noecho();
	keypad(stdscr,TRUE);
	curs_set(0); /* 0, 1, 2 */
	cbreak();
	//scrollok(stdscr,TRUE);
	//start_color();
	move(0,0);
	refresh();
	return 1;
}

//int	init_default_programs(char* config_path, char* TEXT, char* AUDIO, char* VIDEO, char* IMAGE, char* DOC, char* SHELL, char* TERMINAL)
int init_default_programs(char* config_path)
{
	FILE *fp;
	int n;
	int m;
	char cfg_line[255];
	char filetype[64];
	char program[PATH_MAX];

	fp = fopen(config_path,"r");
	if (!fp) {
		endwin();
		fprintf(stderr, "ERROR: Could not locate at default location $USER/.config/moonrabbit/config.\nPut one there or specify a full path with -c option.)\n");
		perror("fopen");
		exit(EXIT_FAILURE);
	}
	n = 0;
	m = 0;

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
		else if (!strcmp(filetype,"SHELL"))
			strcpy(SHELL,program);
		else if (!strcmp(filetype,"TERMINAL"))
			strcpy(TERMINAL,program);
		}
	}

	fclose(fp);
    return 1;
}

int parse_config(char* config_path)
{
	FILE *stream;
	int n;
	int m;
	char *line = NULL;
	ssize_t len = 0;
	size_t nread;
	unsigned char reading_programs = 0;
	unsigned char reading_file_types = 0;
	unsigned short reading_extensions = 0;

	char* current_program_path = NULL;

	stream = fopen(config_path,"r");
	if (!stream) {
		endwin();
		fprintf(stderr, "ERROR: Could not locate at default location $USER/.config/moonrabbit/config.\nPut one there or specify a full path with -c option.)\n");
		perror("fopen");
		exit(EXIT_FAILURE);
	}

	ct_str_init(&ct);

	while ((nread = getline(&line, &len, stream)) != -1) {
		if (line[0] == '#' || line[0] == '\n') {
		}
		else if (line[0] == '}') {
			reading_extensions = 0;
		}
		else if (line[0] == '[') {
			int k = 0;
			while (line[k] != ']') {
				k++;
			}
			k--;
			char section_title[k];
			section_title[k] = '\0';
			for (int i = 0; i < k; i++) {
				section_title[i] = line[i+1];
			}
			if (!strcmp(section_title,"Programs")) {
				reading_programs = 1;
				reading_file_types = 0;
				reading_extensions = 0;
			}
			else if (!strcmp(section_title,"File Types")) {
				reading_programs = 0;
				reading_file_types = 1;
				reading_extensions = 0;
			}
			else if (!strcmp(section_title,"End")) {
				reading_programs = 0;
				reading_file_types = 0;
				reading_extensions = 0;
			}
		}
		else {
			if (reading_programs) {

			}
			else if (reading_extensions) { // This HAS TO BE BEFORE *else if (reading_file_types)*
				int start = 0;
				int end = 0;
				int count = 0;

				for (int i = 0; i < strlen(line); i++) {
					if (line[i] == ' ' || line[i] == '\t') {
						continue;
					}
					else if (line[i] == '.') {
						int start = i;
						i++;
						while (line[i] != '.' && i < strlen(line)) {
							i++;
						}
						int end = i;
						int ext_len = end - start - 1;
						//char *ext = malloc(sizeof(char) * ext_len);
						char ext[ext_len];
						ext[ext_len] = '\0';
						for (int j = 0; j < ext_len; j++) {
							ext[j] = line[start+j];
						}
						ct_str_ins_into_list(&ct, current_program_path, ext);
						i--;
					}
				}
			}
			else if (reading_file_types) {
				int k = 0;
				while (line[k] != '{') {
					k++;
				}
				//char * program_path = malloc(sizeof(char)*k);
				char program_path[k];
				for (int i = 0; i < k; i++) {
					program_path[i] = line[i];
				}
				program_path[k] = '\0';
				ct_str_add_new_list(&ct, program_path);
				reading_extensions = 1;
				if (current_program_path == (char*) NULL) {
					free(current_program_path);
				}
				current_program_path = malloc(sizeof(char) * k);
				strcpy(current_program_path, program_path);
			}
		}
	}
	free(line);
	fclose(stream);
	return 1;
}

int set_default_programs() 
{
	FILE* stream = fopen("/home/gab/temp123","w");
	char n[12];
	sprintf(n,"%d",ct.size);
	fputs(n,stream);
	fputs("\n",stream);

	for (int i = 0; i < ct.size; i++) {
		fputs(ct.list[i].title, stream);

		node_str* temp = ct.list[i].head;
		while (temp != NULL) {
			fputs(" -> ",stream);
			fputs(temp->data,stream);
			temp = temp->next;
		}
		fputs("\n",stream);
	};

	fclose(stream);
	return 0;
}


#endif

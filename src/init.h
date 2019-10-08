#ifndef INIT_H
#define INIT_H
#include <ncurses.h>

#define MAX_PROG_PATH_LEN 64
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

int	init_default_programs(char* config_path, char* TEXT, char* AUDIO, 
	char* VIDEO, char* IMAGE, char* DOC, char* SHELL, char* TERMINAL)
{
	FILE *fp;
	int n;
	int m;
	char cfg_line[255];
	char filetype[64];
	char program[64];

	fp = fopen(config_path,"r");
	if (!fp) {
		endwin();
		fprintf(stderr, "ERROR: Could not locate at default location $USER/.config/moonrabbit/config. Put one there or specify a full path with -c option.)\n");
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

#endif

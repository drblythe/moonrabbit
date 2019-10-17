#ifndef INIT_H
#define INIT_H
#include <ncurses.h>
#include <limits.h>
#include "utils.h"
#include "chained_table.h"

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
						char ext[ext_len];
						ext[ext_len] = '\0';
						for (int j = 0; j < ext_len; j++) {
							ext[j] = line[start+j];
						}
						str_remove_outer_ws(ext);
						ct_str_ins_into_list(&ct, current_program_path, ext);
						i--;
					}
				}
			}
			else if (reading_file_types) {
				int exec_in_term = line[0] == '$' ? 1 : 0;

				int k = 0;
				while (line[k] != '{') {
					k++;
				}
				char program_path[k-exec_in_term];
				for (int i = 0; i < k-exec_in_term; i++) {
					program_path[i] = line[i+exec_in_term];
				}
				program_path[k-exec_in_term] = '\0';
				str_remove_outer_ws(program_path);
				ct_str_add_new_list(&ct, program_path);
				if (exec_in_term == 1) {
					ct.list[ct.size-1].exec_in_term = 1;
				}
				else {
					ct.list[ct.size-1].exec_in_term = 0;
				}
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

#endif

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <limits.h>
#include <ncurses.h>

#include "entry.h"
#include "file_handling.h"
#include "input.h"
#include "command_handling.h"
#include "bindings.h"


int main(int argc, char* argv[])
{
	// Declare globals
	//
	int c, x, y;
	WINDOW* win = NULL;
	ENTRY* entry_arr = NULL;
	KEY_VALUE* stored_indexes = malloc(sizeof(KEY_VALUE)*MAX_STORED_INDEXES);
	KEY_VALUE** p_stored_indexes = &stored_indexes;
	int num_stored_indexes = 0;
	char* cwd;
	char *config_path;
	char **input;
	int current_index;
	int num_entries;
	int show_dots;

	// Storage for program exec paths
	//
	char TEXT[64]; 
	char AUDIO[64]; 
	char VIDEO[64]; 
	char IMAGE[64]; 
	char DOC[64]; 
	char SHELL[64];
	char TERMINAL[64];

	// Init ncurses
	//
	if (!init_ncurses(win)) {
		endwin();
		fprintf(stderr, "Error: failed to init ncurses window\n");
		exit(EXIT_FAILURE);
	}

	// Get current working directory
	//
	cwd = malloc(sizeof(char) * PATH_MAX);
	argc > 1 ? strcpy(cwd,argv[1]) : (cwd = getcwd(NULL,0));
	if (!cwd) {
		endwin();
		perror("getcwd");
		exit(EXIT_FAILURE);
	}

	// Initialize other various globals
	//
	show_dots = 0;
	current_index = 0;
	num_entries = 0;
	getyx(stdscr, y, x);

	// Init moonrabbit stuff (read config for program prefs, get files in dir, etc.)
	//
	get_entries(cwd, &entry_arr, &num_entries, show_dots);
	display_entries(entry_arr, num_entries, current_index,LINES);
	display_file_info(cwd, entry_arr[current_index],current_index, num_entries);
	config_path = "/home/charlie/prog/moonrabbit/config";
	refresh(); /* wrefresh(stdscr); */
	set_default_programs(config_path, TEXT, AUDIO, VIDEO, IMAGE, DOC, SHELL, TERMINAL);

	// Main loop
	// 	
	int run = 1;
	while(run)
	{
		flushinp();
		getyx(stdscr, y, x);
		c = getch();
		switch(c) {

		case KEY_DOWN:
		case ('j'):
			if (current_index < num_entries -1 ) {
				update_curr_index(DOWN, &current_index, &num_entries);
				display_entries(entry_arr, num_entries, current_index,LINES);
				display_file_info(cwd, entry_arr[current_index],current_index, num_entries);
				refresh();
			}
			break;

		case KEY_UP:
		case 'k':
			if (current_index > 0 ) {
				update_curr_index(UP, &current_index, &num_entries);
				display_entries(entry_arr, num_entries, current_index,LINES);
				display_file_info(cwd, entry_arr[current_index],current_index, num_entries);
				refresh();
			}
			break;

		case KEY_LEFT:
		case 'h':
			if (strcmp(cwd, "/")) {
				if (save_index(p_stored_indexes, num_stored_indexes, cwd, current_index))
					num_stored_indexes++;
				prev_dir(&cwd);
				clear_entries(entry_arr, &num_entries, &current_index,1);
				load_index(p_stored_indexes, num_stored_indexes, cwd, &current_index);
				get_entries(cwd, &entry_arr, &num_entries, show_dots);
				display_entries(entry_arr, num_entries, current_index,LINES);
				display_file_info(cwd, entry_arr[current_index],current_index, num_entries);
				refresh();
			}
			break;

		case K_ENTER:
		case 'l':
			if (entry_arr[current_index].type == 'd') {
				/*
				for (int i = 0; i < num_stored_indexes; i++) {
					printf("/////dir=%s,index=%d//////",stored_indexes[i].key, stored_indexes[i].value );
					getchar();
				}
				*/
				if (save_index(p_stored_indexes, num_stored_indexes, cwd, current_index))
					num_stored_indexes++;
				next_dir(&cwd, entry_arr[current_index].name);
				clear_entries(entry_arr, &num_entries, &current_index, 1);
				load_index(p_stored_indexes, num_stored_indexes, cwd, &current_index);
				get_entries(cwd, &entry_arr, &num_entries, show_dots);
			}
			else if (entry_arr[current_index].type != 'd') {
				open_file(cwd, entry_arr[current_index].name,TEXT,AUDIO,VIDEO,IMAGE,DOC,SHELL,TERMINAL);

			}
			else {
				/* damn */
			}
			clear();
			display_entries(entry_arr, num_entries, current_index,LINES);
			display_file_info(cwd, entry_arr[current_index],current_index, num_entries);
			refresh();
			break;

		case ':':
			input = malloc(sizeof(char)*128);
			*input = get_input();
			handle_cmd(input,&cwd);
			free(input);
			clear_entries(entry_arr, &num_entries, &current_index,1);
			get_entries(cwd, &entry_arr, &num_entries, show_dots);
			erase();
			display_entries(entry_arr, num_entries, current_index,LINES);
			display_file_info(cwd, entry_arr[current_index],current_index, num_entries);
			refresh();
			break;

		case '/':
			input = malloc(sizeof(char)*128);
			*input = get_input();
			if (search_dir(*input,entry_arr,&current_index,num_entries)) {
				erase();
				display_entries(entry_arr, num_entries, current_index,LINES);
				display_file_info(cwd, entry_arr[current_index],current_index, num_entries);
				refresh();
			}
			free(input);
			break;

		case 'g':
			if ( (c = getch()) == 'g'){
				current_index = 0;
				erase();
				display_entries(entry_arr, num_entries, current_index,LINES);
				display_file_info(cwd, entry_arr[current_index],current_index, num_entries);
				refresh();
			}
			break;

		case 'G':
			current_index = num_entries -1;
			erase();
			clear();
			display_entries(entry_arr, num_entries, current_index,LINES);
			display_file_info(cwd, entry_arr[current_index],current_index, num_entries);
			refresh();
			break;

		case 'q':
			run = 0;
			break;

		case 'S':
			// open shell here
			break;

		case ctrl('h'):
			(!show_dots) ?  (show_dots = 1) : (show_dots = 0);
			clear_entries(entry_arr, &num_entries, &current_index,1);
			get_entries(cwd, &entry_arr, &num_entries, show_dots);
			display_entries(entry_arr, num_entries, current_index,LINES);
			display_file_info(cwd, entry_arr[current_index],current_index, num_entries);
			break;

		
		case ctrl('r'):
			erase();
			//mvprintw(10,20,"--keycode=%d--",c);
			display_entries(entry_arr, num_entries, current_index,LINES);
			display_file_info(cwd, entry_arr[current_index],current_index, num_entries);
			refresh();
			break;


		case K_SPACE:
			if (!entry_arr[current_index].marked)
				mark_file(&entry_arr[current_index]);
			else
				unmark_file(&entry_arr[current_index]);
			if (current_index < num_entries -1) 
				update_curr_index(DOWN, &current_index, &num_entries);
			else 
				erase();
			display_entries(entry_arr, num_entries, current_index,LINES);
			display_file_info(cwd, entry_arr[current_index],current_index, num_entries);
			refresh();
			break;

		default:
			break;
		}
	}

	// Free up malloced mem, clear screen, close
	free(entry_arr);
	free(stored_indexes);
	move(0,0);
	erase();
	refresh();
	endwin();

	/* So GCC will shut up */
	x += y;
	num_stored_indexes--;
	p_stored_indexes++;

	exit(EXIT_SUCCESS);
}

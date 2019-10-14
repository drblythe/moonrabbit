#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <limits.h>
#include <ncurses.h>
#include <locale.h>

#include "entry.h"
#include "file_handling.h"
#include "input.h"
#include "command_handling.h"
#include "bindings.h"
#include "init.h"


int main(int argc, char* argv[])
{
	// Declare globals
	//
	int c, x, y;
	WINDOW* win = NULL;
	ENTRY* entry_arr = NULL;

	char **copy_buffer = NULL;// put these in a struct
	// bool in_use;
	int copy_buff_del = 0;//
	int copy_buff_size = 0;//

	int num_selected = 0;

	KEY_VALUE* stored_indexes = malloc(sizeof(KEY_VALUE)*MAX_STORED_INDEXES);
	KEY_VALUE** p_stored_indexes = &stored_indexes;
	int num_stored_indexes = 0;
	char* cwd;
	//char *config_path;
	char config_path[PATH_MAX];
	char **input;
	int current_index;
	int num_entries;
	int show_dots;

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

	/* Config Path */
	int user_cfg = 0;
	while (argc > 1) {
		if (argv[1][0] == '-') {
			if (argv[1][1] == 'c') {
				user_cfg = 1;
			}
			else {
				fprintf(stderr, "Error: unrecognized option %s\n", argv[1]);
				exit(EXIT_FAILURE);
			}
			argc--;
			argv++;
		}
		else
			break;
	}
	if (user_cfg) {
		strcpy(config_path,argv[1]);
	}
	else {
		char username[128] ;
		getlogin_r(username,128);
		strcpy(config_path, "/home/");
		strcat(config_path, username);
		strcat(config_path, "/.config/moonrabbit/config");
	}

	// Init moonrabbit stuff (read config for program prefs, get files in dir, etc.)
	//
	get_entries(cwd, &entry_arr, &num_entries, show_dots);
	display_entries(entry_arr, num_entries, current_index,LINES);
	display_file_info(cwd, entry_arr[current_index],current_index, num_entries);
	refresh(); /* wrefresh(stdscr); */
	//init_default_programs(config_path);
	parse_config(config_path);
	set_default_programs();

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
#if 0
			if (strcmp(cwd, "/")) {
#endif
				if (save_index(p_stored_indexes, num_stored_indexes, cwd, current_index))
					num_stored_indexes++;
				prev_dir(&cwd);
				clear_entries(entry_arr, &num_entries, &current_index,1);
				load_index(p_stored_indexes, num_stored_indexes, cwd, &current_index);
				get_entries(cwd, &entry_arr, &num_entries, show_dots);
				display_entries(entry_arr, num_entries, current_index,LINES);
				display_file_info(cwd, entry_arr[current_index],current_index, num_entries);
				refresh();
	//		}
			break;

		case K_ENTER:
		case 'l':
			if (!num_entries)
				break;
			if (entry_arr[current_index].type == 'd') {
				if (save_index(p_stored_indexes, num_stored_indexes, cwd, current_index))
					num_stored_indexes++;
				next_dir(&cwd, entry_arr[current_index].name);
				clear_entries(entry_arr, &num_entries, &current_index, 1);
				load_index(p_stored_indexes, num_stored_indexes, cwd, &current_index);
				get_entries(cwd, &entry_arr, &num_entries, show_dots);
				num_selected = 0;
			}
			else if (entry_arr[current_index].type != 'd') {
				open_file(cwd, entry_arr[current_index].name, &ct, TEXT,AUDIO,VIDEO,IMAGE,DOC,SHELL,TERMINAL);

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
			//if (search_dir(*input,entry_arr,&current_index,num_entries)) {
			if (binarysearch_entry(*input, entry_arr, num_entries, &current_index)) {
				erase();
				display_entries(entry_arr, num_entries, current_index,LINES);
				display_file_info(cwd, entry_arr[current_index],current_index, num_entries);
				refresh();
			}
			free(input);
			break;

		// Copy
		case 'y':
			if ( (c = getch()) == 'y'){
				copy_buff_size = get_num_marked(num_entries, entry_arr);
				fill_copy_buffer(&copy_buffer, copy_buff_size, num_entries, entry_arr, cwd);
				copy_buff_del = 0;
			}
			break;

		// Cut
		case 'd':
			if ( (c = getch()) == 'd'){
				copy_buff_size = get_num_marked(num_entries, entry_arr);
				fill_copy_buffer(&copy_buffer, copy_buff_size, num_entries, entry_arr, cwd);
				copy_buff_del = 1;
			}
			break;

		// Paste
		case 'p':
			if ( (c = getch()) == 'p'){
				file_buff_cp(copy_buff_del, cwd, copy_buffer, &copy_buff_size);
				empty_copy_buffer(&copy_buffer, &copy_buff_size);
				clear_entries(entry_arr, &num_entries, &current_index,1);
				get_entries(cwd, &entry_arr, &num_entries, show_dots);
				erase();
				display_entries(entry_arr, num_entries, current_index,LINES);
				display_file_info(cwd, entry_arr[current_index],current_index, num_entries);
				refresh();
			}
			break;

		case 'x':
			if ( (c = getch()) == 'x' && num_selected > 0) {
				int ret = confirm_deletion(LINES-4, 0, num_selected);
				if (ret) {
					copy_buff_size = get_num_marked(num_entries, entry_arr);
					fill_copy_buffer(&copy_buffer, copy_buff_size, num_entries, entry_arr, cwd);
					copy_buff_del = 0;
					delete_selection(copy_buffer, &copy_buff_size);
					empty_copy_buffer(&copy_buffer, &copy_buff_size);
					clear_entries(entry_arr, &num_entries, &current_index,1);
					get_entries(cwd, &entry_arr, &num_entries, show_dots);
					erase();
					display_entries(entry_arr, num_entries, current_index,LINES);
					display_file_info(cwd, entry_arr[current_index],current_index, num_entries);
					refresh();
					num_selected = 0;
				}
			}
			break;

		case 'g':
			if ( (c = getch()) == 'g'){
				current_index = 0;
				//erase();
				clear();
				display_entries(entry_arr, num_entries, current_index,LINES);
				display_file_info(cwd, entry_arr[current_index],current_index, num_entries);
				//refresh();
			}
			break;


		case 'G':
			current_index = num_entries -1;
			//erase();
			clear();
			display_entries(entry_arr, num_entries, current_index,LINES);
			display_file_info(cwd, entry_arr[current_index],current_index, num_entries);
			//refresh();
			break;

		case 'q':
			run = 0;
			break;

		case 'S':
			// open shell here
			break;

		//case ctrl('h'):
		case 'H':
			(!show_dots) ?  (show_dots = 1) : (show_dots = 0);
			clear_entries(entry_arr, &num_entries, &current_index,1);
			get_entries(cwd, &entry_arr, &num_entries, show_dots);
			display_entries(entry_arr, num_entries, current_index,LINES);
			display_file_info(cwd, entry_arr[current_index],current_index, num_entries);
			break;

		
		//case ctrl('r'):
		case 'r':
			clear();
			display_entries(entry_arr, num_entries, current_index,LINES);
			display_file_info(cwd, entry_arr[current_index],current_index, num_entries);
			break;


		case K_SPACE:
			if (!entry_arr[current_index].marked) 
				mark_file(&entry_arr[current_index],&num_selected);
			else 
				unmark_file(&entry_arr[current_index],&num_selected);
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
	//clear();
	erase();
	refresh();
	endwin();

	/* So GCC will shut up */
	x += y;
	num_stored_indexes--;
	p_stored_indexes++;

	exit(EXIT_SUCCESS);
}

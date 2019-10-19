#define _XOPEN_SOURCE_EXTENDED

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
	int c, x, y;
	WINDOW* win = NULL;
	ENTRY* entry_arr = NULL;
	char **copy_buffer = NULL;
	int copy_buff_del = 0;
	int copy_buff_size = 0;
	int num_selected = 0;
	index_table stored_indexes;
	char* cwd;
	char config_path[PATH_MAX];
	char **input;
	int current_index;
	int num_entries;
	int show_dots;



	// Init ncurses
	
	cwd = malloc(sizeof(char) * PATH_MAX);
	cwd = getcwd(NULL,0);
	if (!cwd) {
		endwin();
		perror("getcwd");
		exit(EXIT_FAILURE);
	}
	show_dots = 0;
	current_index = 0;
	num_entries = 0;
	getyx(stdscr, y, x);

	/* Config Path */
	int user_cfg = 0;
	while (argc > 1) {
		if (argv[1][0] == '-') {
			if (argv[1][1] == 'c') {
				if (argc > 2) {
					user_cfg = 1;
					strcpy(config_path, argv[2]);
					argc--;
					argv++;
				}
				else {
					fprintf(stderr, "%s -c [/path/to/config]\n", argv[0]);
					exit(EXIT_FAILURE);
				}
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
	if (!user_cfg) {
		char username[128] ;
		getlogin_r(username,128);
		strcpy(config_path, "/home/");
		strcat(config_path, username);
		strcat(config_path, "/.config/moonrabbit/config");
	}

	if (!init_ncurses(win)) {
		endwin();
		fprintf(stderr, "Error: failed to init ncurses window\n");
		exit(EXIT_FAILURE);
	}


	// Init moonrabbit stuff (read config for program prefs, get files in dir, etc.)
	get_entries(cwd, &entry_arr, &num_entries, show_dots);
	display_entries(entry_arr, num_entries, current_index,LINES);
	display_file_info(cwd, entry_arr[current_index],current_index, num_entries);
	refresh(); /* wrefresh(stdscr); */
	parse_config(config_path);
	index_table_init(&stored_indexes);



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
			index_table_store(&stored_indexes, cwd, current_index);
			prev_dir(&cwd);
			clear_entries(entry_arr, &num_entries, &current_index,1);
			index_table_load(&stored_indexes, cwd, &current_index);
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
				index_table_store(&stored_indexes, cwd, current_index);
				next_dir(&cwd, entry_arr[current_index].name);
				clear_entries(entry_arr, &num_entries, &current_index, 1);
				index_table_load(&stored_indexes, cwd, &current_index);
				get_entries(cwd, &entry_arr, &num_entries, show_dots);
				num_selected = 0;
			}
			else if (entry_arr[current_index].type != 'd') {
				open_file(cwd, entry_arr[current_index].name, &ct);
			}
			else {
			}
			clear();
			display_entries(entry_arr, num_entries, current_index,LINES);
			display_file_info(cwd, entry_arr[current_index],current_index, num_entries);
			refresh();
			break;

		case ':':
			input = malloc(sizeof(char)*128);
			*input = get_input(0);
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
			*input = get_input(1);
			search_dir(*input,entry_arr,&current_index,num_entries);
			//binarysearch_entry(*input, entry_arr, num_entries, &current_index);
			erase();
			display_entries(entry_arr, num_entries, current_index,LINES);
			display_file_info(cwd, entry_arr[current_index],current_index, num_entries);
			refresh();
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

		case ctrl('h'):
		case 'H':
			(!show_dots) ?  (show_dots = 1) : (show_dots = 0);
			clear_entries(entry_arr, &num_entries, &current_index,1);
			get_entries(cwd, &entry_arr, &num_entries, show_dots);
			clear();
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

	free(entry_arr);
	index_table_free(&stored_indexes);
	move(0,0);
	clear();
	erase();
	refresh();
	endwin();
	x += y;
	exit(EXIT_SUCCESS);
}

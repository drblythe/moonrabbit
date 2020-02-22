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

// TODO:
// - In my "shell": mkdir, touch, etc. all need to be appended to cwd instead of
//	 dir where moonrabbit was started from.
// - Scrolling: Why don't the selected/bolded dirs scroll WITH the rest???
// - When changing directory: If current index > num entries, set it to num entries

int main(int argc, char* argv[])
{
	WINDOW* win = NULL;
	int input_key;
	char **copy_buffer = NULL;
	int copy_buff_del = 0;
	int copy_buff_size = 0;
	int num_selected = 0;
	ENTRY* entry_arr = NULL;
	index_table stored_indexes;
	int current_index = 0;
	int num_entries = 0;
	bool show_dots = 0;
	char* cwd;
	char config_path[PATH_MAX];
	char **input;
	bool exec_in_term = false;

	// Ensure ncurses initializies
	if (!init_ncurses(win)) {
		endwin();
		fprintf(stderr, "Error: failed to init ncurses window\n");
		exit(EXIT_FAILURE);
	}

	// Start moonrabbit in current working dir
	cwd = getcwd(NULL,0);
	if (!cwd) {
		endwin();
		perror("getcwd");
		exit(EXIT_FAILURE);
	}

	// Set config path, 
	parse_cmd_args(argc, argv, config_path);
	parse_config_file(config_path);
	index_table_init(&stored_indexes);

	// Initialize moonrabbit display
	get_entries(cwd, &entry_arr, &num_entries, show_dots);
	display_entries(entry_arr, num_entries, current_index,LINES);
	display_file_info(cwd, entry_arr[current_index],current_index, num_entries);
	refresh(); // wrefresh(stdscr);

	bool run = 1;
	while(run)
	{
		if (exec_in_term) continue;
		flushinp();
		refresh();
		input_key = getch();
		switch(input_key) {

		// Move down
		case KEY_DOWN:
		case ('j'):
			if (current_index < num_entries -1 ) {
				update_curr_index(DOWN, &current_index, &num_entries);
				display_entries(entry_arr, num_entries, current_index,LINES);
				display_file_info(cwd, entry_arr[current_index],current_index, num_entries);
				refresh();
			}
			break;

		// Move up
		case KEY_UP:
		case 'k':
			if (current_index > 0 ) {
				update_curr_index(UP, &current_index, &num_entries);
				display_entries(entry_arr, num_entries, current_index,LINES);
				display_file_info(cwd, entry_arr[current_index],current_index, num_entries);
				refresh();
			}
			break;

		// Previous directory
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
			break;

		// Open file/directory
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
				open_file(&exec_in_term, cwd, entry_arr[current_index].name, &ct);
			}
			else {
			}
			clear();
			display_entries(entry_arr, num_entries, current_index,LINES);
			display_file_info(cwd, entry_arr[current_index],current_index, num_entries);
			refresh();
			break;

		// Interactive/command prompt
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

		// Search
		case '/':
			input = malloc(sizeof(char)*128);
			*input = get_input(1);
			search_dir(*input,entry_arr,&current_index,num_entries);
			erase();
			display_entries(entry_arr, num_entries, current_index,LINES);
			display_file_info(cwd, entry_arr[current_index],current_index, num_entries);
			refresh();
			free(input);
			break;

		// Copy
		case 'y':
			if ( (input_key = getch()) == 'y'){
				copy_buff_size = get_num_marked(num_entries, entry_arr);
				fill_copy_buffer(&copy_buffer, copy_buff_size, num_entries, entry_arr, cwd);
				copy_buff_del = 0;
			}
			break;

		// Cut
		case 'd':
			if ( (input_key = getch()) == 'd'){
				copy_buff_size = get_num_marked(num_entries, entry_arr);
				fill_copy_buffer(&copy_buffer, copy_buff_size, num_entries, entry_arr, cwd);
				copy_buff_del = 1;
			}
			break;

		// Paste
		case 'p':
			if ( (input_key = getch()) == 'p'){
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

		// Delete (permanent)
		case 'x':
			if ( (input_key = getch()) == 'x' && num_selected > 0) {
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

		// Move cursor to top
		case 'g':
			if ( (input_key = getch()) == 'g'){
				current_index = 0;
				clear();
				display_entries(entry_arr, num_entries, current_index,LINES);
				display_file_info(cwd, entry_arr[current_index],current_index, num_entries);
			}
			break;

		// Move cursor to bottom
		case 'G':
			current_index = num_entries -1;
			clear();
			display_entries(entry_arr, num_entries, current_index,LINES);
			display_file_info(cwd, entry_arr[current_index],current_index, num_entries);
			break;

		// Quit
		case 'q':
			run = 0;
			break;

		// TODO: open shell here
		case 'S':
			break;

		// Toggle hidden files
		case ctrl('h'):
		case 'H':
			(!show_dots) ?  (show_dots = 1) : (show_dots = 0);
			clear_entries(entry_arr, &num_entries, &current_index,1);
			get_entries(cwd, &entry_arr, &num_entries, show_dots);
			clear();
			display_entries(entry_arr, num_entries, current_index,LINES);
			display_file_info(cwd, entry_arr[current_index],current_index, num_entries);
			break;

		// Refresh moonrabbit display
		case 'r':
			clear();
			display_entries(entry_arr, num_entries, current_index,LINES);
			display_file_info(cwd, entry_arr[current_index],current_index, num_entries);
			break;

		// Select/deselect file
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

	// Free globals / storage structures
	free(entry_arr);
	free(cwd);
	free(win);
	index_table_free(&stored_indexes);
	ct_str_free_table(&ct);

	// Clear/reset term settings for clean exit
	move(0,0);
	clear();
	erase();
	refresh();
	endwin();

	exit(EXIT_SUCCESS);
}

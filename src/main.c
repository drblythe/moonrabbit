#define _XOPEN_SOURCE_EXTENDED
#include <limits.h>
#include <graphics.h>
#include <file_operations.h>
#include <cmd_handling.h>
#include <init.h>

// TODO:
// - In the "shell": mkdir, touch, etc. all need to be appended to cwd instead of
//	 dir where moonrabbit was started from.
// - When changing directory: If current index > num entries, set it to num entries
// - Something wrong w/ displaying of cwd name (at bottom) when its length is 
// 	 greater than the term width

int main(int argc, char* argv[])
{
	WINDOW* win = NULL;
	ext_table ct;
	int input_key;
	char **copy_buffer = NULL;
	int copy_buff_del = 0;
	int copy_buff_size = 0;
	int num_selected = 0;
	file_t* file_arr = NULL;
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
	parse_config_file(config_path, &ct);
	index_table_init(&stored_indexes);

	// Initialize moonrabbit display
	get_file_list(cwd, &file_arr, &num_entries, show_dots);
	refresh_display(file_arr, num_entries, current_index, LINES, cwd);

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
				refresh_display(file_arr, num_entries, current_index, LINES, cwd);
			}
			break;

		// Move up
		case KEY_UP:
		case 'k':
			if (current_index > 0 ) {
				update_curr_index(UP, &current_index, &num_entries);
				refresh_display(file_arr, num_entries, current_index, LINES, cwd);
			}
			break;

		// Previous directory
		case KEY_LEFT:
		case 'h':
			store_index(&stored_indexes, cwd, current_index);
			prev_dir(&cwd);
			clear_file_list(file_arr, &num_entries, &current_index,1);
			load_index(&stored_indexes, cwd, &current_index);
			get_file_list(cwd, &file_arr, &num_entries, show_dots);
			refresh_display(file_arr, num_entries, current_index, LINES, cwd);
			break;

		// Open file/directory
		case K_ENTER:
		case KEY_RIGHT:
		case 'l':
			if (!num_entries)
				break;
			if (file_arr[current_index].type == 'd') {
				store_index(&stored_indexes, cwd, current_index);
				next_dir(&cwd, file_arr[current_index].name);
				clear_file_list(file_arr, &num_entries, &current_index, 1);
				load_index(&stored_indexes, cwd, &current_index);
				get_file_list(cwd, &file_arr, &num_entries, show_dots);
				num_selected = 0;
			}
			else if (file_arr[current_index].type != 'd') {
				open_file(&exec_in_term, cwd, file_arr[current_index].name, &ct);
			}
			else {
			}
			clear();
			refresh_display(file_arr, num_entries, current_index, LINES, cwd);
			break;

		// Interactive/command prompt
		case ':':
			input = malloc(sizeof(char)*128);
			*input = get_input(0);
			handle_cmd(input,&cwd);
			free(input);
			clear_file_list(file_arr, &num_entries, &current_index,1);
			get_file_list(cwd, &file_arr, &num_entries, show_dots);
			erase();
			refresh_display(file_arr, num_entries, current_index, LINES, cwd);
			break;

		// Search
		case '/':
			input = malloc(sizeof(char)*128);
			*input = get_input(1);
			search_dir(*input,file_arr,&current_index,num_entries);
			erase();
			refresh_display(file_arr, num_entries, current_index, LINES, cwd);
			free(input);
			break;

		// Copy
		case 'y':
			if ( (input_key = getch()) == 'y'){
				copy_buff_size = get_num_marked(num_entries, file_arr);
				fill_copy_buffer(&copy_buffer, copy_buff_size, num_entries, file_arr, cwd);
				copy_buff_del = 0;
			}
			break;

		// Cut
		case 'd':
			if ( (input_key = getch()) == 'd'){
				copy_buff_size = get_num_marked(num_entries, file_arr);
				fill_copy_buffer(&copy_buffer, copy_buff_size, num_entries, file_arr, cwd);
				copy_buff_del = 1;
			}
			break;

		// Paste
		case 'p':
			if ( (input_key = getch()) == 'p'){
				file_buff_cp(copy_buff_del, cwd, copy_buffer, &copy_buff_size);
				empty_copy_buffer(&copy_buffer, &copy_buff_size);
				clear_file_list(file_arr, &num_entries, &current_index,1);
				get_file_list(cwd, &file_arr, &num_entries, show_dots);
				erase();
				refresh_display(file_arr, num_entries, current_index, LINES, cwd);
			}
			break;

		// Delete (permanent)
		case 'x':
			if ( (input_key = getch()) == 'x' && num_selected > 0) {
				int ret = confirm_deletion(LINES-4, 0, num_selected);
				if (ret) {
					copy_buff_size = get_num_marked(num_entries, file_arr);
					fill_copy_buffer(&copy_buffer, copy_buff_size, num_entries, file_arr, cwd);
					copy_buff_del = 0;
					delete_selection(copy_buffer, &copy_buff_size);
					empty_copy_buffer(&copy_buffer, &copy_buff_size);
					clear_file_list(file_arr, &num_entries, &current_index,1);
					get_file_list(cwd, &file_arr, &num_entries, show_dots);
					erase();
					refresh_display(file_arr, num_entries, current_index, LINES, cwd);
					num_selected = 0;
				}
			}
			break;

		// Move cursor to top
		case 'g':
			if ( (input_key = getch()) == 'g'){
				current_index = 0;
				clear();
				refresh_display(file_arr, num_entries, current_index, LINES, cwd);
			}
			break;

		// Move cursor to bottom
		case 'G':
			current_index = num_entries -1;
			clear();
			refresh_display(file_arr, num_entries, current_index, LINES, cwd);
			break;

		// Quit
		case 'q':
			run = 0;
			break;

		// TODO: open shell here
		case 'S':
			break;

		// Toggle hidden files
		case 'H':
		case K_CTRL_H:
			(!show_dots) ?  (show_dots = 1) : (show_dots = 0);
			clear_file_list(file_arr, &num_entries, &current_index,1);
			get_file_list(cwd, &file_arr, &num_entries, show_dots);
			clear();
			refresh_display(file_arr, num_entries, current_index, LINES, cwd);
			break;

		// Refresh moonrabbit display
		case 'r':
		case K_CTRL_R:
			clear();
			refresh_display(file_arr, num_entries, current_index, LINES, cwd);
			break;

		// Select/deselect file
		case K_SPACE:
			if (!file_arr[current_index].marked) 
				mark_file(&file_arr[current_index],&num_selected);
			else 
				unmark_file(&file_arr[current_index],&num_selected);
			if (current_index < num_entries -1) 
				update_curr_index(DOWN, &current_index, &num_entries);
			else 
				erase();
			refresh_display(file_arr, num_entries, current_index, LINES, cwd);
			break;

		default:
			break;
		}
	}

	// Free globals / storage structures
	free(file_arr);
	free(cwd);
	free(win);
	index_table_free(&stored_indexes);
	ext_table_free(&ct);

	// Clear/reset term settings for clean exit
	reset_term();
	exit(EXIT_SUCCESS);
}

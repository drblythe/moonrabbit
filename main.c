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
#include "dir_info.h"
#include "input.h"
#include "command_handling.h"
#include "bindings.h"

//#define ctrl(x)		((x) & 0x1f)
//#define KEY_SPACE	' '

int main()
{
	int c, x, y;
	WINDOW* win = NULL;
	ENTRY* entry_arr;
	U_DIR* dir_arr;
	//int dirs_stored;

	char* cwd;
	//char* ext;
	char *config_path;
	char *input;
	int current_index;
	int num_entries;
	int show_dots;

	if (!init_ncurses(win)) {
		endwin();
		fprintf(stderr, "Error: failed to init ncurses window\n");
		exit(EXIT_FAILURE);
	}

	cwd = malloc(sizeof(char) * PATH_MAX);
	cwd = getcwd(NULL,0);
	if (!cwd) {
		endwin();
		perror("getcwd");
		exit(EXIT_FAILURE);
	}
	int run = 1;

	//dirs_stored = 0;
	dir_arr = malloc(sizeof(U_DIR) * 32);
	show_dots = 0;
	current_index = 0;
	num_entries = 0;
	getyx(stdscr, y, x);
	get_entries(cwd, &entry_arr, &num_entries, show_dots);
	display_entries(entry_arr, num_entries, current_index,LINES);
	display_file_info(cwd, entry_arr[current_index],current_index, num_entries);
	config_path = "/home/haru/documents/moonrabbit/config";
	set_default_programs(config_path);
	//move(0,0);
	//mvprintw(LINES-1,0,"%s/%s",cwd, entry_arr[current_index].name);
	//mvprintw(LINES-2,0,"index:%d // total entries:%d(-1) // LINES: %d // ncurses cursor pos: y=%d",
	//	current_index,num_entries,LINES,y);

	//move(y,x);
	refresh(); /* wrefresh(stdscr); */

	//FILE *fp;
   	//char line[255];
	while(run)
	{
		flushinp();
		getyx(stdscr, y, x);
		c = getch();
		switch(c) {
		case KEY_DOWN:
			c = 'j';
		case ('j'):
			if (current_index < num_entries -1 ) {
				update_curr_index(DOWN, &current_index, &num_entries);
				display_entries(entry_arr, num_entries, current_index,LINES);
				display_file_info(cwd, entry_arr[current_index],current_index, num_entries);
				refresh();
			}

			break;
		case KEY_UP:
			c = 'k';
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
				prev_dir(&cwd);
				clear_entries(entry_arr, &num_entries, &current_index);
				get_entries(cwd, &entry_arr, &num_entries, show_dots);
				display_entries(entry_arr, num_entries, current_index,LINES);
				display_file_info(cwd, entry_arr[current_index],current_index, num_entries);
				refresh();
			}
			break;
		case 'i':
			refresh();
			break;

		case KEY_RIGHT:
		case 'l':
			if (entry_arr[current_index].type == 'd') {
				next_dir(&cwd, entry_arr[current_index].name);
			}
			else if (entry_arr[current_index].type != 'd') {
				//ext = get_extension(entry_arr[current_index].name);
				//free(ext);
				open_file(cwd, entry_arr[current_index].name);
				erase();
			}
			else {
				/* damn */
			}
			erase();
			clear_entries(entry_arr, &num_entries, &current_index);
			get_entries(cwd, &entry_arr, &num_entries, show_dots);
			erase();
			refresh();
			break;

		case ':':
			input = malloc(sizeof(char)*128);
			input = get_input();
			handle_cmd(input,&cwd);
			free(input);
			erase();
			clear_entries(entry_arr, &num_entries, &current_index);
			get_entries(cwd, &entry_arr, &num_entries, show_dots);
			display_entries(entry_arr, num_entries, current_index,LINES);
			display_file_info(cwd, entry_arr[current_index],current_index, num_entries);
			refresh();
			break;

		case '/':
			input = malloc(sizeof(char)*128);
			input = get_input();
			// find_in_dir(char *name);
			// move_to_entry();
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
			display_entries(entry_arr, num_entries, current_index,LINES);
			display_file_info(cwd, entry_arr[current_index],current_index, num_entries);
			refresh();
			break;

		case 'q':
			run = 0;
			break;
		
		case 'S':
			open_shell(cwd);
			break;

		case ctrl('h'):
			(!show_dots) ?  (show_dots = 1) : (show_dots = 0);
			clear_entries(entry_arr, &num_entries, &current_index);
			get_entries(cwd, &entry_arr, &num_entries, show_dots);
			display_entries(entry_arr, num_entries, current_index,LINES);
			display_file_info(cwd, entry_arr[current_index],current_index, num_entries);
			break;
		case KEY_SPACE:
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

	free(entry_arr);
	move(0,0);
	erase();
	refresh();
	endwin();
	exit(EXIT_SUCCESS);
}

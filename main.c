/*
TODO:
	-	Check permissions before chdir or opening file
	-	Store index from previous/forward directories
	- 	Optionally hide dot files
	-	Scrolling:
			upward scrolling
	- 	Fix open_file urxvt thing

DONE(?):
	-	Weird cursor position thing where scrolling down too far
		stops letting you move up. Caused by stdscr y value being
		wrong. Somehow it switches direction during j/k presses
	- 	Scroll if num_entries > LINES-1 (HOLY SHIT)
	- 	Dedicate areas to display information/create function
		to display to certain area on screen
	-	~/.steam / ~/.steampath / ~/.steampid permissions were
		weird, stat doesn't return correctly
	- 	Malloc/free entry_arr on every directory change instead of
		using the same max size like a goddamn neanderthal
	-	Some kind of coloring to make dir/reg files distinct
*/
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

int main()
{
	int c, x, y;
	WINDOW* win = NULL;
	ENTRY* entry_arr;
	char* cwd = malloc(sizeof(char) * PATH_MAX);
	int num_entries;
	int current_index, prev_index, next_index;

	if (!init_ncurses(win)) {
		endwin();
		fprintf(stderr, "Error: failed to init ncurses window\n");
		exit(EXIT_FAILURE);
	}

	num_entries = 0;
	current_index = 0;
	next_index = 0;
	prev_index = 0;
	cwd = getcwd(NULL,0);
	if (!cwd) {
		endwin();
		perror("getcwd");
		exit(EXIT_FAILURE);
	}
	int run = 1;

	getyx(stdscr, y, x);
	get_entries(cwd, &entry_arr, &num_entries);
	display_entries(entry_arr, num_entries, current_index,LINES);
	display_file_info(cwd, entry_arr[current_index].name,current_index, num_entries);
	//move(0,0);
	display_file_info(cwd, entry_arr[current_index].name,current_index, num_entries);
	//mvprintw(LINES-1,0,"%s/%s",cwd, entry_arr[current_index].name);
	//mvprintw(LINES-2,0,"index:%d // total entries:%d(-1) // LINES: %d // ncurses cursor pos: y=%d",
	//	current_index,num_entries,LINES,y);

	move(y,x);
	refresh();
	refresh(); /* wrefresh(stdscr); */

	while(run)
	{
		flushinp();
		getyx(stdscr, y, x);
		c = getch();
		switch(c) {
		case 'j':
			if (current_index < num_entries -1 ) {
				update_curr_index(DOWN, &current_index, &num_entries);
				display_entries(entry_arr, num_entries, current_index,LINES);
				display_file_info(cwd, entry_arr[current_index].name,current_index, num_entries);
				move(++y,x);
				refresh();
			}

			break;
		case 'k':
			if (current_index > 0 ) {
				update_curr_index(UP, &current_index, &num_entries);
				display_entries(entry_arr, num_entries, current_index,LINES);
				display_file_info(cwd, entry_arr[current_index].name,current_index, num_entries);
				move(--y,x);
				refresh();
			}

			break;
		case 'h':
			if (strcmp(cwd, "/")) {
				prev_dir(&cwd, &current_index, &prev_index, &next_index);
				clear_entries(entry_arr, &num_entries);
				get_entries(cwd, &entry_arr, &num_entries);
				display_entries(entry_arr, num_entries, current_index,LINES);

				display_file_info(cwd, entry_arr[current_index].name,current_index, num_entries);

				move(y,x);
				move(0,0);
				refresh();
			}
			break;

		case KEY_HOME:
			mvprintw(10,20,"sssssss");
			move(y,x);
			break;

		case 'l':
			if (entry_arr[current_index].is_dir) {
				next_dir(&cwd, entry_arr[current_index].name, &current_index, &prev_index, &next_index);
				clear_entries(entry_arr, &num_entries);
				get_entries(cwd, &entry_arr, &num_entries);
				display_entries(entry_arr, num_entries, current_index,LINES);
				display_file_info(cwd, entry_arr[current_index].name,current_index, num_entries);

				//move(y,x);
				//move(0,0);
			}
			else if (!entry_arr[current_index].is_dir) {
				//open_file(cwd, entry_arr[current_index].name);
				refresh();
			}
			else {
				/*
				ZOMG UR FUCKKED DUDE
				-	if it is a special file or dont have permission
				-	permission needs to be checked from another function
					before attempting to chdir or open file
				*/
			}
			refresh();
			break;

		case 'q':
			run = 0;
			break;

		default:
			break;
		}
	}


	free(cwd);
	free(entry_arr);
	endwin();
	return 0;
}

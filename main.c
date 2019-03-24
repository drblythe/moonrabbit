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

#define ctrl(x)		((x) & 0x1f)
#define KEY_SPACE	' '

/*
TODO:
	- 	Accept the fact that display functions need to be moved
		out of entry.c and be sad for a while
	- 	When ':' is pressed, start accepting input
	-	Add directory with mkdir
	-	Check permissions before chdir or opening file
		https://linux.die.net/man/2/access
		https://linux.die.net/man/2/stat
	-	Store index from previous/forward directories
	-	Scrolling:
			upward scrolling
	- 	Fix open_file urxvt thing
	- 	If text printed is longer than columns-1,
		put ... at the end (or columns-4 i guess)


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
	- 	Optionally hide dot files
	-	Some kind of coloring to make dir/reg files distinct
	-	Mark files with tab/underline/whatever with spacebar
*/


int main()
{
	int c, x, y;
	WINDOW* win = NULL;
	ENTRY* entry_arr;

	char* cwd = malloc(sizeof(char) * PATH_MAX);
	int current_index;
	//int prev_index;
	//int forward_index;
	int num_entries;
	int show_dots;

	if (!init_ncurses(win)) {
		endwin();
		fprintf(stderr, "Error: failed to init ncurses window\n");
		exit(EXIT_FAILURE);
	}

	cwd = getcwd(NULL,0);
	if (!cwd) {
		endwin();
		perror("getcwd");
		exit(EXIT_FAILURE);
	}
	int run = 1;

	show_dots = 0;
	current_index = 0;
	num_entries = 0;
	getyx(stdscr, y, x);
	get_entries(cwd, &entry_arr, &num_entries, show_dots);
	display_entries(entry_arr, num_entries, current_index,LINES);
	//move(0,0);
	display_file_info(cwd, entry_arr[current_index],current_index, num_entries);
	//mvprintw(LINES-1,0,"%s/%s",cwd, entry_arr[current_index].name);
	//mvprintw(LINES-2,0,"index:%d // total entries:%d(-1) // LINES: %d // ncurses cursor pos: y=%d",
	//	current_index,num_entries,LINES,y);

	//move(y,x);
	refresh(); /* wrefresh(stdscr); */

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
			c = 'h';
		case 'h':
			if (strcmp(cwd, "/")) {
				prev_dir(&cwd);
				clear_entries(entry_arr, &num_entries, &current_index);
				get_entries(cwd, &entry_arr, &num_entries, show_dots);
				display_entries(entry_arr, num_entries, current_index,LINES);

				display_file_info(cwd, entry_arr[current_index],current_index, num_entries);

				move(y,x);
				move(0,0);
				refresh();
			}
			break;

		case KEY_RIGHT:
			c = 'l';
		case 'l':
			//if (entry_arr[current_index].type == 'd') {
			if (check_permissions("enter", cwd, entry_arr[current_index])) {
				next_dir(&cwd, entry_arr[current_index].name);
				clear_entries(entry_arr, &num_entries, &current_index);
				get_entries(cwd, &entry_arr, &num_entries, show_dots);
				display_entries(entry_arr, num_entries, current_index,LINES);
				display_file_info(cwd, entry_arr[current_index],current_index, num_entries);
			}
			else if (entry_arr[current_index].type != 'd') {
				//open_file(cwd, entry_arr[current_index].name);
			}
			else {
				/* damn */
			}
			refresh();
			break;

		case 'q':
			run = 0;
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
			if (current_index< num_entries -1) 
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


	free(cwd);
	free(entry_arr);
	endwin();
	exit(EXIT_SUCCESS);
}

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <locale.h>
#include "file_type.h"
#include "keycodes.h"

int init_ncurses(WINDOW *win)
{
	setlocale(LC_ALL, ""); // Set locale to be able to properly display unicode. Must precede initscr()
	win = initscr();
	noecho();
	keypad(stdscr,TRUE);
	curs_set(0); /* 0, 1, 2 */
	cbreak();
	move(0,0);
	refresh();
	return 1;
}

int display_file_list(file_t* file_arr,int num_entries, int current_index,int LINES)
{
	unsigned int RIGHT_PADDING = 4;
	int PRINT_STOP = LINES - 3;
	int SCROLL_STOP_BOTTOM = PRINT_STOP - 5;

	if (!num_entries) {
		printw("<empty directory>");
		return 1;
	}

	// Case 1: Length of file list is <= the number of rows in terminal
	if (num_entries <= PRINT_STOP) {
		for (int i = 0; i < num_entries; i++) {
			if (file_arr[i].index == current_index) 
				wattron(stdscr, A_REVERSE);
			if (file_arr[i].type == 'd')
				wattron(stdscr, A_BOLD);
			if (file_arr[i].marked) {
				printw("   ");
			}
			if ( strlen(file_arr[i].name) >= COLS-RIGHT_PADDING) {
				for (int j = 0; j < COLS-RIGHT_PADDING; j++) {
					printw("%c",file_arr[i].name[j]);
				}
				printw("...\n");
			}
			else
				printw("%s\n",file_arr[i].name);
			wattrset(stdscr, A_NORMAL);
		}
		return 1;
	}
				
	// Case 2: Length of file list displays past bottom of terminal
	else {
		// dist: the distance your cursor is below the initial bottom row
		int dist = current_index - SCROLL_STOP_BOTTOM;
		if (dist < 0)
			dist = 0;

		// Reverse is for cursor: always reverse the line that cursor is on
		// Bold is for directories: scroll them with the screen (index is i+distance from bottom)
		// ^^ Same as bold for marked files
		for (int i = 0; i <= PRINT_STOP; i++) {
			if (dist > 0)
				current_index = SCROLL_STOP_BOTTOM;
			if (file_arr[i].index == current_index) 
				wattron(stdscr, A_REVERSE);
			if (file_arr[i+dist].type == 'd')
				wattron(stdscr, A_BOLD);
			if (file_arr[i+dist].marked) {
				printw("   ");
			}
			if ( strlen(file_arr[i+dist].name) >= COLS-RIGHT_PADDING) {
				for (int j = 0; j < COLS-RIGHT_PADDING; j++) {
					printw("%c",file_arr[i+dist].name[j]);
				}
				printw("...\n");
			}
			else
				printw("%s\n",file_arr[i+dist].name);
			wattrset(stdscr, A_NORMAL);
		}
		return 1;
	}
	return -1;
}


int display_file_info(char* cwd, file_t file, int current_index, int num_entries)
{
	int RIGHT_PADDING = 0 + 5;
	if (!num_entries)
		return 0;
	char* perm = get_permissions(cwd, file.name);
	int path_len = strlen(cwd) + 1 + 1 + strlen(file.name) + 1;
	mvprintw(LINES-2, 0, "%d/%d  %c%s", current_index+1, num_entries, file.type,perm);
	free(perm);

	// Print path
	// If full path is less than the width of the term, just print it
	wattron(stdscr,A_BOLD);
	if (path_len < COLS-RIGHT_PADDING) {
		mvprintw(LINES-1, 0, "%s",cwd);
		wattroff(stdscr,A_BOLD);
		if (strcmp(cwd,"/")) {
			mvprintw(LINES-1, strlen(cwd), "/", file.name);
			mvprintw(LINES-1, strlen(cwd)+1, "%s", file.name);
		}
		else {
			mvprintw(LINES-1, strlen(cwd), "%s", file.name);
		}
	}
	// If full path is greater than width of term, 
	else {
		char path[path_len];
		strcpy(path,cwd);
		if (!strcmp(cwd,"/")) {
			strcat(path, "/");
		}
		strcat(path, file.name);
		for (int i = 0; i < COLS-RIGHT_PADDING; i++) {
			if (i > strlen(cwd)) {
				wattroff(stdscr,A_BOLD);
			}
			mvprintw(LINES-1, i, "%c", path[i]);
		}
		mvprintw(LINES-1, COLS-RIGHT_PADDING, "...");
	}
	return 1;
}



char* get_input(bool search_mode)
{
	char * command;
	command = malloc(sizeof(char) * 255);
	// clear info area and put your colon on display
	for (int i = 0; i < COLS-1; i++) {
		mvprintw(LINES-2,i, " ");
		mvprintw(LINES-1,i, " ");
	}
	if (search_mode) {
		mvprintw(LINES-1,0, "/");
	}
	else {
		mvprintw(LINES-1,0, ":");
	}

	// Turn terminal feedback on
	echo();
	nocbreak();

	// While char isn't ENTER, get char
	int c, read, cmd_index;
	read = 1;
	cmd_index = 0;
	
	while (read) {
		c = getch();
		switch (c) {
		case K_ENTER:
			read = 0;
			break;
		case K_BACKSPACE:
			command[cmd_index] = '\0';
			cmd_index--;
			break;
		default:
			command[cmd_index] = c;
			cmd_index++;
			command[cmd_index] = '\0';
			break;
		}
	}
	
	// Turn terminal feedback off again
	noecho();
	cbreak();
	return command;
}

// Print confirmation prompt for file deletion
bool confirm_deletion(int x_pos, int y_pos, int num_selected)
{
	mvprintw(x_pos, y_pos, "Delete selection? (y/N)", num_selected);
	char c = getch();
	if (c == 'y' || c == 'Y')
		return 1;
	else 
		return 0;
}


void refresh_display(file_t *file_arr, int num_entries, int curr_index, int LINES, char *cwd)
{
	display_file_list(file_arr, num_entries, curr_index, LINES);
	display_file_info(cwd, file_arr[curr_index], curr_index, num_entries);
	refresh();
}

// Reset terminal colors/cursor visibility
void reset_term()
{
	move(0,0);
	clear();
	erase();
	refresh();
	endwin();
}


#endif

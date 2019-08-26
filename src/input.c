/* input.c */
#include "input.h"

char* get_input(void)
{
	char * command;
	command = malloc(sizeof(char) * 255);
	/* clear info area and put your colon on display */
	for (int i = 0; i < COLS-1; i++) {
		mvprintw(LINES-2,i, " ");
		mvprintw(LINES-1,i, " ");
	}
	mvprintw(LINES-1,0, ":");

	/* turn terminal feedback on */
	echo();
	nocbreak();

	/* while char isnt ENTER, get char*/
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
		/*
		case 'q':
			read = 0;
			break;
		*/
		default:
			command[cmd_index] = c;
			cmd_index++;
			command[cmd_index] = '\0';
			break;
		}
	}
	
	/* turn terminal feedback off again */
	noecho();
	cbreak();
	return command;
}

int confirm_deletion(int x_pos, int y_pos, int num_selected)
{
	//mvprintw(x_pos, y_pos, "Delete %d files? (y/N)", num_selected);
	mvprintw(x_pos, y_pos, "Delete selection? (y/N)", num_selected);
	char c = getch();
	if (c == 'y' || c == 'Y')
		return 1;
	else 
		return 0;
}

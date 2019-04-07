/* commands.c */
#include "input.h"

char* get_cmd(void)
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
		case ENTER:
			read = 0;
			break;
		case BACKSPACE:
			command[cmd_index] = '\0';
			cmd_index--;
			break;
		case 'q':
			read = 0;
			break;
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

int handle_cmd(char* command)
{
	//if (!strcmp(command, "q"))
		//exit(EXIT_SUCCESS);
		// add a new directory named
	return 1;

}



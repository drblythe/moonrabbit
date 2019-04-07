/* commands.h */
#include <ncurses.h>
#include <string.h>
#include <stdlib.h>

#define ENTER 10
#define BACKSPACE 263

char* get_cmd(void);
int handle_cmd(char *command);

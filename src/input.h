/* input.h */
#ifndef INPUT_H
#define INPUT_H
#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include "bindings.h"

char* get_input(bool search_mode);
int search(char *phrase);
bool confirm_deletion(int x_pos, int y_pos, int num_selected);
#endif

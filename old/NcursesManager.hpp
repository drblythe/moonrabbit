#include <ncurses.h>
#include <string>
#include <iostream>

namespace Ncurses {

static int 		init();
static int 		terminate();
static int 		print(std::string str,int xPos, int yPos);
static std::string getInput();
static int 		clear();

};

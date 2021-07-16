#include "NcursesManager.hpp"

namespace Ncurses {


static int init()
{
	initscr();			/* Start curses mode 		*/
	raw();				/* Line buffering disabled	*/
	keypad(stdscr, TRUE);		/* We get F1, F2 etc..		*/
	noecho();			/* Don't echo() while we do getch */
	
	return 0;
}

static int terminate()
{
	return endwin();
}

static int print(std::string str,int xPos, int yPos)
{
	mvprintw(xPos, yPos, str.c_str());
	refresh();
	return 0;
}

static std::string getInput()
{
	std::string input = "";
	input.push_back(getch());
	return input;
}

static int clear()
{
	return clear();
}

}

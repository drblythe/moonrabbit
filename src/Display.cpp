#include "Display.hpp"

namespace Curses {

Display::Display()
{

}

int 
Display::init()
{
	initscr();					/* Start curses mode 		*/
	raw();						/* Line buffering disabled	*/
	keypad(stdscr, TRUE);		/* We get F1, F2 etc..		*/
	noecho();					/* Don't echo() while we do getch */
	
	return 0;
}

int 
Display::print(std::string text, int row, int col)
{
	mvprintw(row, col, text.c_str());
	refresh();
	return 0;
}

int 
Display::clear()
{
	return clear();
}

int 
Display::terminate()
{
	return endwin();
}


}

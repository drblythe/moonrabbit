#include "Display.hpp"
#include <curses.h>

namespace Curses
{

Display::Display()
{
    init();
}

int
Display::init()
{
    initscr();                  /* Start curses mode        */
    raw();                      /* Line buffering disabled  */
    keypad(stdscr, TRUE);       /* We get F1, F2 etc..      */
    noecho();                   /* Don't echo() while we do getch */

    return 0;
}

int
Display::print(const std::string &text, int row, int col, attr_t attr)
{
	// TODO: Is it okay to apply A_NORMAL every print?
	if (!attr == A_NORMAL) attron(attr);
    mvprintw(row, col, text.c_str());
	if (!attr == A_NORMAL) attroff(attr);
    refresh();
    return 0;
}

int
Display::clearAll()
{
    return clear();
}

int
Display::terminate()
{
    return endwin();
}


}

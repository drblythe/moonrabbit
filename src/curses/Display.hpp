#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <ncurses.h>
#include <string>

namespace Curses
{
class Display
{
public:
    Display();
    int         print(const std::string &text, int row, int col, attr_t attr = A_NORMAL);
    int         clearAll();
    int         terminate();
private:
    int         init();
	WINDOW* 	win;
};

static Display *DISPLAY = new Display();

}

#endif

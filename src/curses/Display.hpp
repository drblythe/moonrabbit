#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <ncurses.h>
#include <string>

namespace Curses {
class Display {
	public:
		Display();
		int 		print(std::string text, int row, int col);
		int 		clear();
		int 		terminate();
	private:
		int 		init();
};

static Display *DISPLAY = new Display();

}

#endif

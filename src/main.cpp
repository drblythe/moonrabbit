#include "curses/Display.hpp"
#include "curses/Input.hpp"

#include <iostream>
#include <string>

using namespace Curses;

int main (int argc, char* argv[])
{
	std::string input;
	while (input != "q") {
		input = INPUT->getKey();
		DISPLAY->print(input, 0, 10);
	}

	DISPLAY->terminate();

	return 0;
}

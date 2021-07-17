#include "Input.hpp"

namespace Curses {
Input::Input()
{

}

std::string 
Input::getKey()
{
	std::string input = "";
	input.push_back(getch());
	return input;
}

int 
Input::listen()
{
	return 0;
}


}

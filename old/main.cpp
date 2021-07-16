#include "NcursesManager.hpp"
#include "System/Input.hpp"

int main()
{	

	Ncurses::init();
	
	int run = 0;
	while (!run){
		run = System::Input::listen();
	}
	Ncurses::terminate();

	return 0;
}

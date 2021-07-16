#include "Input.hpp"

namespace System {
namespace Input {


static int listen()
{

	Ncurses::print("Type any character to see it in bold\n", 0, 0);

	/*int ch = getch();*/			/* If raw() hadn't been called
					 			 * we have to press enter before it
					 			 * gets to the program 		*/

	if(getch() == KEY_F(1))		
	{

		/* Without keypad enabled this will */
		/*  not get to us either	*/
		/* Without noecho() some ugly escape
		 * charachters might have been printed
		 * on screen			*/
		Ncurses::print("F1 Key pressed", 0, 1);
		return 1;
	}
	else
	{	
		std::string msg = "The pressed key is ";
		Ncurses::print(msg, 1, 0);
		attron(A_BOLD);
		Ncurses::print(Ncurses::getInput(), 1, msg.length());
		attroff(A_BOLD);
		getch();
		clear();
		return 0;
	}

}

}}

#include "signal_handling.h"

// Reset terminal colors/cursor visibility
void reset_term()
{
	move(0,0);
	clear();
	erase();
	refresh();
	endwin();
}

// Callback for catch_signals
void signal_handler(int signo)
{
	switch (signo) {
	case(SIGWINCH):
		// refresh the display && don't terminate
        break;
    case(SIGINT):
        reset_term();
		exit(EXIT_SUCCESS);
        break;
    case(SIGTERM):
        reset_term();
		exit(EXIT_SUCCESS);
        break;
    case (SIGCHLD):
		//printf("MY CHIIIILD\n");
        // just don't terminate!
        break;
	default:
		//printf("SIG...Whatever\n");
		break;
	}
}

// Called from a while loop
// Prevent instant termination on child terminations, window size changes, interrupts
void catch_signals()
{
	signal(SIGWINCH, signal_handler);
	signal(SIGINT,signal_handler);
	signal(SIGCHLD, signal_handler);
	sleep(1);
}
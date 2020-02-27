#include "../include/signal_handling.h"

// Callback for catch_signals
void signal_handler(int signo)
{
	switch (signo) {
	case(SIGWINCH):
        break;
    case(SIGINT):
		exit(EXIT_SUCCESS);
        break;
    case(SIGTERM):
		exit(EXIT_SUCCESS);
        break;
    case (SIGCHLD):
        // just don't terminate!
        break;
	default:
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
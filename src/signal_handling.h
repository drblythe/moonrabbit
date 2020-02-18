#ifndef SIGNAL_HANDLING_H
#define SIGNAL_HANDLING_H
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include "ncurses.h"
#include <stdlib.h>

void reset_term();
void signal_handler(int signo);
void catch_signals();

#endif
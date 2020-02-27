#ifndef SIGNAL_HANDLING_H
#define SIGNAL_HANDLING_H
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void signal_handler(int signo);
void catch_signals();

#endif
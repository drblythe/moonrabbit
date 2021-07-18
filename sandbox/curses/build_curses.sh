#!/bin/bash

g++ \
	-o test_curses \
	-l ncurses \
	-std=c++17 \
	-Wall \
	-Wextra \
	../../src/curses/*.cpp \
	../../src/system/*.cpp \
	./main.cpp



	#-Werror \

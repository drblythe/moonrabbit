#!/bin/bash

g++ \
	-o a.out \
	-l ncurses \
	-std=c++17 \
	-Wall \
	-Wextra \
	src/curses/*.cpp \
	src/system/*.cpp \
	src/main.cpp

	#-Werror \

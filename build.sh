#!/bin/bash

g++ -lncurses \
	src/curses/Display.cpp \
	src/curses/Input.cpp \
	src/curses/Curses.cpp \
	src/main.cpp \
	-g -Wall \
	-o bin/foo

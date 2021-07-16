#!/bin/bash

g++ -lncurses \
	src/Display.cpp \
	src/Input.cpp \
	src/main.cpp \
	-g -Wall \
	-o bin/foo

bin/foo

#!/bin/bash

g++ \
	-o test_system \
	-std=c++17 \
	-Wall \
	-Wextra \
	../src/system/*.cpp \
	./test_system.cpp



	#-Werror \

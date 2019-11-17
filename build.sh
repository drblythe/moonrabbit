#!/bin/bash

gcc $(ncursesw6-config --cflags --libs) ./src/*.c -o moonrabbit

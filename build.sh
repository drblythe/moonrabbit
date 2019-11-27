#!/bin/bash

gcc src/*c $(ncursesw6-config --cflags --libs) -o moonrabbit

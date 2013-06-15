#!/bin/bash

#should really get a makefile going, this is fine for just one test file for now.
gcc capture.c -I/usr/include/libftdi1 -lftdi1

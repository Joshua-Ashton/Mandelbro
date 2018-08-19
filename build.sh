#!/bin/bash

gcc -Ofast -fopenmp -fomit-frame-pointer -static -static-libgcc -static-libstdc++ -flto mandelbro.c -o mandelbro
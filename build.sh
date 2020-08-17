#!/bin/sh
g++ main.cpp -o raytracer.out
./raytracer.out > image.ppm
gwenview image.ppm
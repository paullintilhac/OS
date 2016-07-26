#!/bin/bash

g++ -std=gnu++11 -o src/io/sched src/io/*.cpp

chmod +x src/io/sched

cd TestData

../src/io/sched -si input0 
#./runit.sh myout

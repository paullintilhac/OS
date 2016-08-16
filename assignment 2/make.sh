#!/bin/bash


g++ -std=gnu++11 -o sched *.cpp
chmod +x sched

#./sched -sR2 -v lab2_assign/input2 lab2_assign/rfile

cd lab2_assign

./runit.sh
./diffit.sh . refout

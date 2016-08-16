#!/bin/bash


g++ -std=gnu++11 -o sched *.cpp
chmod +x sched

#./sched -sP2 -v lab2_assign/input3 lab2_assign/rfile

cd lab2_assign

./runit.sh
./diffit.sh . refout

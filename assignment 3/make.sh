#!/bin/bash


g++ -std=gnu++11 -o mmu *.cpp
chmod +x mmu
./mmu -aX -oOPFS lab3_assign/in18 lab3_assign/rfile

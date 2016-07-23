#!/bin/bash


g++ -std=gnu++11 -o mmu *.cpp
chmod +x mmu
./mmu -aa -f4 -oOPFS lab3_assign/in18 lab3_assign/rfile

#./runit_one.sh myout in18 4 OPFS ./mmu
#./gradeit.sh myout outputs

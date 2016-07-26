#!/bin/bash


g++ -std=gnu++11 -o mmu *.cpp
chmod +x mmu
#./mmu -aN -f8 -oOPFS in60 rfile

#./runit_one.sh myout in18  4 OPFS ./mmu

#./gradeit.sh myout outputs

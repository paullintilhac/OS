#!/bin/bash

g++ -std=gnu++11 -o src/io/sched src/io/*.cpp

chmod +x src/io/sched

cd TestData

#../src/io/sched -sc input0


#echo "************"
#echo reference output
#cat out_0_c_long

./runit.sh myout
./gradeit.sh myout .

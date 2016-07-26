#!/bin/bash

g++ -std=gnu++11 -o src/io/sched src/io/*.cpp

chmod +x src/io/sched

cd TestData

../src/io/sched -si input0

echo "************"
echo reference output
cat out_0_i

#./runit.sh myout

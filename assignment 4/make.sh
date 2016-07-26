#!/bin/bash

g++ -std=gnu++11 -o src/io/sched src/io/*.cpp

chmod +x src/io/sched

cd TestData

../src/io/sched -sj input0

echo "************"
echo reference output
cat out_0_j

#./runit.sh myout

#!/bin/bash

if [ -e $1.cpp ]; then
a=" file exists, compiling..."
echo $1.cpp$a
else
a=" file does not exist, cannot compile"
echo $1.cpp$a
exit 1
fi

g++ -o $1.out $1.cpp

./$1.out

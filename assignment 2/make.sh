#!/bin/bash

if [ -e $1 ]; then
a=" file exists, compiling..."
echo $1$a
else
a=" file does not exist, cannot compile"
echo $1$a
exit 1
fi

g++ -o lab2.out *.cpp *.h
chmod +x lab2.out
./lab2.out $1 $2

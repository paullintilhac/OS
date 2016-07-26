#!/bin/bash

g++ -std=gnu++11 -o iosched *.cpp

chmod +x iosched

./iosched -sf TestData/input0 

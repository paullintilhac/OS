#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <stdlib.h>
#include "Rand.h"
using namespace std;

Rand::Rand(string randfile){

	ifstream randStream (randfile.c_str());
	string numberCount;
	getline(randStream,numberCount);
    int numCount = atoi(numberCount.c_str());

	this->numRand = numCount;
	int vec[numCount];
	string str;
	for (int i=0;i<numCount;i++){
	    vec[i] = atoi(str.c_str());
	}
	this->randVec = vec;
	this->randCount = 0;
}


int Rand::myrandom(int burst){
    int returnVal= 1+this->randVec[this->randCount%this->numRand]%burst;
    randCount++;
    return returnVal;
}



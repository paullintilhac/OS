#ifndef RAND_H
#define RAND_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <stdlib.h>
using namespace std;
class Rand{
public:
	Rand(string randfile){
		ifstream randStream (randfile.c_str());
		string numberCount;
		getline(randStream,numberCount);
	    int numCount = atoi(numberCount.c_str());

		this->numRand = numCount;
		int vec[numCount];
		string str;
		for (int i=0;i<numCount;i++){
		 	getline(randStream,str);
		    vec[i] = atoi(str.c_str());
		}
		this->randVec = vec;
		this->randCount = 0;
		randStream.close();
		};


		myrandom(int burst){
		    int returnVal= randVec[randCount%numRand]%burst;
		    //cout<<"random val: "<<returnVal<<endl;
		    randCount++;
		    return returnVal;
		}


		int randCount;
		int numRand;
		int* randVec;
	};
	


#endif
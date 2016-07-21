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
		cout<<"set numRand to "<<numCount<<endl;
		};


		myrandom(int burst){
			cout<<" burst: "<<burst<<endl;
			cout<<"num rand: "<<numRand<<endl;	
		//	cout<<"randCount: "<<randCount<<", numRand: "<<numRand<<", burst: "<<burst<<endl;
		    int returnVal= randVec[randCount%numRand]%burst;
		    randCount++;
		    return returnVal;
		}


		int randCount;
		int numRand;
		int* randVec;
	};
	


#endif
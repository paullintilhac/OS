#ifndef TABLES_H
#define TABLES_H
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <memory>
#include "Rand.h"
using namespace std;
class Page{
public:
	Page(){
		PRESENT = 0;
		MODIFIED = 0;
		REFERENCED = 0;
		PAGEDOUT = 0;
		frame = 0;
	}
	unsigned int PRESENT:1;
	unsigned int MODIFIED:1;
	unsigned int REFERENCED:1;
	unsigned int PAGEDOUT:1;
	unsigned int frame:10;
	
};

class Frame{
public:
	Frame(){
		referenceCount = 0;
		locked = false;
		page = new Page();
		pageNum = -1;
	}
	int pageNum;
	Page *page;
	int referenceCount;
	bool locked;
	//also linkage, not sure what that is
};

class Class{
public:
	Class(int nFrames, Rand* r):nFrames{nFrames}{
		classVec = new bool[nFrames][4];
		for (int i=0; i<4;i++){
			for (int j=0;j<nFrames;++j){
				if (i==0){
					classVec[j][i]=true;
				} else{
					classVec[j][i]=false;
				}
			}
		}
		counts[0] = nFrames;
		rand = r;

	}
	int nFrames;
	int counts[4];
	int lowestClass;

	bool (*classVec)[4];
	Rand* rand;
	update(Page pte){
		for (int i=0;i<4;++i){
			classVec[pte.frame][i]=false;
		}

		if (pte.REFERENCED==0 && pte.MODIFIED==0){
			classVec[pte.frame][0]=true;
		}
		if (pte.REFERENCED==1 && pte.MODIFIED==0){
			classVec[pte.frame][1]=true;

		}
		if (pte.REFERENCED==0 && pte.MODIFIED==1){
			classVec[pte.frame][2]=true;
		}
		if (pte.REFERENCED==1 && pte.MODIFIED==1){
			classVec[pte.frame][3]=true;
		}
		for (int i=0;i<4;++i){
			int sum = 0;
			for (int j=0;j<nFrames;++j){
				if (classVec[j][i]){
					sum++;
				}
			}
			counts[i]=sum;
		}
			
			
	}

	int get_page_number(){
		int lowClass = 0;
		for (int i=0;i<4;++i){
			lowClass = i;
			if (counts[i]>0){
				break;
			}
		}
		lowestClass = lowClass;
		int frameIndex = rand->myrandom(counts[lowestClass]);
		//cout<<"lowest class: "<<lowestClass<<", randval: "<<frameIndex<<endl;
		return frameIndex;
	}

};


#endif
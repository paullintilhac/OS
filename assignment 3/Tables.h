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
		class_0 = new bool[nFrames];
		class_1 = new bool[nFrames];
		class_2 = new bool[nFrames];
		class_3 = new bool[nFrames];
		for (int i=0; i<nFrames;i++){
			class_0[i]=true;
			class_1[i]=false;
			class_2[i]=false;
			class_3[i]=false;
		}
		counts[0] = nFrames;
		rand = r;

	}
	int nFrames;
	int counts[4];
	bool *class_0, *class_1, *class_2,*class_3;
	Rand* rand;
	update(Page pte){
		class_0[pte.frame]=false;
		class_1[pte.frame]=false;
		class_2[pte.frame]=false;
		class_3[pte.frame]=false;
		

		if (pte.REFERENCED==0 && pte.MODIFIED==0){
			class_0[pte.frame]=true;
		}
		if (pte.REFERENCED==1 && pte.MODIFIED==0){
			class_2[pte.frame]=true;

		}
		if (pte.REFERENCED==0 && pte.MODIFIED==1){
			class_1[pte.frame]=true;
		}
		if (pte.REFERENCED==1 && pte.MODIFIED==1){
			class_3[pte.frame]=true;
		}
			int sum=0;
			for (int j=0;j<nFrames;++j){
				if (class_0[j]){
					sum++;
				}
			}
			counts[0]=sum;
			sum=0;
			for (int j=0;j<nFrames;++j){
				if (class_1[j]){
					sum++;
				}
			}
			counts[1]=sum;
			sum=0;
			for (int j=0;j<nFrames;++j){
				if (class_2[j]){
					sum++;
				}
			}
			counts[2]=sum;
			sum=0;
			for (int j=0;j<nFrames;++j){
				if (class_3[j]){
					sum++;
				}
			}
			counts[3]=sum;
			
			//for (int i=0;i<4;++i){
			//	cout<<"counts["<<i<<"]: "<<counts[i]<<endl;
			//}
			
	}

	int get_page_number(){
		int lowestClass = 0;
		for (int i=0;i<4;++i){
			lowestClass = i;
			if (counts[i]>0){
				break;
			}
		}
		int frameIndex = rand->myrandom(counts[lowestClass]);
		//cout<<"lowest class: "<<lowestClass<<", randval: "<<frameIndex<<endl;
		return frameIndex;
	}

};


#endif
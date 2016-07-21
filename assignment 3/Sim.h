#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Tables.h"
#include "Pager.h"
using namespace std;
class Sim{
public:
	Sim(string inputFileName,int nf, string algoName, bool physical, bool O, bool P, bool F, bool S);
	Page page_table[64];
	Frame* frame_table;
	int nFrames;
	Pager* pager;
	int instructNumber;
};

Sim::Sim(string inputFileName,int nf, string algoName,bool physical,bool o, bool p, bool f, bool s){

    nFrames = nf;
    cout<<"nf: "<<nf<<endl;
   	frame_table= new Frame[nf];
   	if (algoName=="LRU"){
   		pager = new LRU(frame_table,nf);
   	}
	
	cout<<"inputFileName: "<<inputFileName<<endl;
	ifstream infile (inputFileName.c_str());
	string str;
	instructNumber = 0;
		while (getline(infile, str)) {

		++instructNumber;
	    istringstream iss(str);

	    int write;
	    int pageNum;
	 
	    if (!(iss >> write >>pageNum)) { 
	    	cout<<"iss is false in if statement, iss value: "<<iss.fail()<<endl;
	    	break;
	    } 
	    if (o){
	    cout<<"==> inst: "<<write<<" "<<pageNum<<endl;
		}
	    if (page_table[pageNum].PRESENT == 0){
	    	//cout<<"not present"<<endl;
	    	int frameIndex = pager->get_frame();
	    	//cout<<"frame index: "<<frameIndex<<", page num: "<<pageNum<<endl;

	    	//cout<<"frame index: "<<frameIndex<<endl;
	    	Page *oldPage = (frame_table[frameIndex].page);
	    	if (frame_table[frameIndex].pageNum>=0){//-1 will be set before the first reference
	    	//cout<<"page num not -1"<<endl;
	    	if (o){
	    	printf("%d: UNMAP  %2d %2d\n",instructNumber,frame_table[frameIndex].pageNum,oldPage->frame);}
			pager->unmap(oldPage);//unmap previous virtual page associated with selected frame
	    	}

	    	if (oldPage->MODIFIED == 1){
	    		//cout<<"modified"<<endl;
	    		pager->page_out(frame_table[frameIndex]);//page out if dirty
	    		if (o){
	    		printf("%d: OUT    %2d %2d\n",instructNumber,frame_table[oldPage->frame].pageNum,oldPage->frame);
	    		}
	    	}

	    	if (oldPage->PAGEDOUT ==1){
	    		//cout<<"paged out"<<endl;
	    		if (o) {printf("%d: IN     %2d %2d\n",instructNumber,pageNum,frameIndex);}
	    		pager->page_in(frame_table[frameIndex]); //page in if paged out
	    	}

	    	else{
	    		//cout<<"paged in"<<endl;	
	    		if (o){printf("%d: ZERO    %4d\n",instructNumber,frameIndex);}
    		    pager->zero(frame_table[frameIndex]); //zero if already paged in
	    	}
	    	
	    	if (o) {printf("%d: MAP    %2d %2d\n",instructNumber,pageNum,frameIndex);}
	    	pager->map(&(page_table[pageNum]),frameIndex,pageNum);
	    }
	    pager->update_pte(&(page_table[pageNum]),write);

	    //reset every 10 references
	    if (pager->referenceCount==10){
	    	for (int i=0; i<(sizeof(page_table)/sizeof(page_table[0]));++i){
	    		page_table[i].REFERENCED =0;
	    	}
	    }

	}
	if (p){
		for (int i=0;i<(sizeof(page_table)/sizeof(page_table[0]));++i){
			if (page_table[i].PRESENT==1){
				string rmsString = "";
				if (page_table[i].REFERENCED == 1){
					rmsString<<"R";
				} else{ rmsString<<"-";}
				if (page_table[i].MODIFIED == 1){
					rmsString<<"M";
				} else{rmsString<<"-";}
				if (page_table[i].PAGEDOUT ==1){
					rmsString<<"S";
				} else{rmsString<<"-";}
				cout<<i<<":"<<rmsString<<" ";
			} else{
				if (page_table[i].PAGEDOUT ==1){
					cout<<"# ";
				} else{
					cout<<"* ";
				}
			}
		}
	}
	
}

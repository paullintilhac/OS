#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Tables.h"
#include "Pager.h"
#include "Rand.h"
using namespace std;
class Sim{
public:
	Sim(string inputFileName,int nf, string algoName, bool physical, bool O, bool P, bool F, bool S, Rand* rand);
	Page page_table[64];
	Frame* frame_table;
	int nFrames;
	Pager* pager;
	int instructNumber;
	int unmaps;
	int maps;
	int zeros;
	int ins;
	int outs;

};

Sim::Sim(string inputFileName,int nf, string algoName,bool physical,bool o, bool p, bool f, bool s,Rand* rand){
	instructNumber = 0;
	unmaps=0;
	maps = 0;
	zeros=0;
	ins = 0;
	outs = 0;
    nFrames = nf;
   	frame_table= new Frame[nf];
   	Pager* pager;
   	if (algoName=="NRU"){
   		cout<<"using NRU algo"<<endl;
   	   	pager= new NRU(frame_table,page_table, nf, rand);
   	}
   	if (algoName =="LRU"){
   		pager = new LRU(frame_table,nf,rand);
   	}
   	if (algoName == "Random"){
   		pager = new Random(frame_table,nf,rand);
   	}
   	if (algoName == "Clock"){
   		pager = new Clock(frame_table,page_table,nf,rand, physical);
   	}
   	if (algoName == "FIFO"){
   		pager = new FIFO(frame_table,nf,rand);
   	}
   	if (algoName == "SC"){
   		pager = new SC(frame_table,nf,rand);
   	}
   	if (algoName == "Aging"){
   		pager = new Aging(frame_table,page_table,nf,rand);
   	}
	//cout<<"inputFileName: "<<inputFileName<<endl;
	ifstream infile (inputFileName.c_str());
	string str;
		while (getline(infile, str)) {
		/*
		//reset r bits for each page individually based on time since last reference>10
	    for (int i=0; i<(sizeof(page_table)/sizeof(page_table[0]));++i){
	    	if (pager->referenceCount-page_table[i].referenceCount==9){
	    	page_table[i].REFERENCED =0;
	    	pager->classes.update(page_table[i]);
	    	}
	    }
	    */
	    
	    //reset all bits every 10 references globally
	    /*
		if (pager->referenceCount==9){
	    	for (int i=0; i<(sizeof(page_table)/sizeof(page_table[0]));++i){
	    		page_table[i].REFERENCED =0;
	    		pager->classes.update(page_table[i]);
	    	}
	    }
	    */
	    
	   // cout<<"***************"<<endl;
	    //for (int i=0;i<4;i++){
		//cout<<"R bit for frame "<<i<<": "<<frame_table[i].page->REFERENCED<<endl;	
	    //}
	    
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

	    	Page *oldPage = (frame_table[frameIndex].page);
	    	if (frame_table[frameIndex].pageNum>=0){//-1 will be set before the first reference
	    	//cout<<"page num not -1"<<endl;
	    	if (o){
	    	printf("%d: UNMAP   %2d %2d\n",instructNumber,frame_table[frameIndex].pageNum,oldPage->frame);}
			pager->unmap(oldPage);//unmap previous virtual page associated with selected frame
			unmaps++;
	    	}

	    	if (oldPage->MODIFIED == 1){
	    		//cout<<"modified"<<endl;
	    		pager->page_out(frame_table[frameIndex]);//page out if dirty
	    		outs++;
	    		if (o){
	    		printf("%d: OUT     %2d %2d\n",instructNumber,frame_table[oldPage->frame].pageNum,oldPage->frame);
	    		}
	    	}

	    	if (oldPage->PAGEDOUT ==1){
	    		//cout<<"paged out"<<endl;
	    		if (o) {printf("%d: IN      %2d %2d\n",instructNumber,pageNum,frameIndex);}
	    		pager->page_in(frame_table[frameIndex]); //page in if paged out
	    		ins++;
	    	}

	    	else{
	    		//cout<<"paged in"<<endl;	
	    		if (o){printf("%d: ZERO     %4d\n",instructNumber,frameIndex);}
    		    pager->zero(frame_table[frameIndex]); //zero if already paged in
    		    zeros++;
	    	}
	    	
	    	if (o) {printf("%d: MAP     %2d %2d\n",instructNumber,pageNum,frameIndex);}
	    	pager->map(&(page_table[pageNum]),frameIndex,pageNum);
	    	maps++;
	    }
	    pager->update_pte(&(page_table[pageNum]),write);
	    //reset every 10 references

	   	pager->classes.update(page_table[pageNum]);

	    		++instructNumber;


	}
	if (p){
		for (int i=0;i<(sizeof(page_table)/sizeof(page_table[0]));++i){
			if (page_table[i].PRESENT==1){
				ostringstream rmsString("");

				if (page_table[i].REFERENCED == 1){
					rmsString<<"R";
				} else{ rmsString<<"-";}
				if (page_table[i].MODIFIED == 1){
					rmsString<<"M";
				} else{rmsString<<"-";}
				if (page_table[i].PAGEDOUT ==1){
					rmsString<<"S";
				} else{rmsString<<"-";}
				cout<<i<<":"<<rmsString.str()<<" ";
			} else{
				if (page_table[i].PAGEDOUT ==1){
					cout<<"# ";
				} else{
					cout<<"* ";
				}
			}
		}
		cout<<endl;
	}

	if (f){
		for (int i=0;i<nFrames;++i){
			if (frame_table[i].pageNum!=-1){
				cout<<frame_table[i].pageNum<<" ";
			}
			else{
				cout<<"* ";
			}
		}
		cout<<endl;
	}
	if (s){
		unsigned int long long totalCost = (400*(maps+unmaps)+3000*(ins+outs)+zeros*150+instructNumber);
		printf("SUM %d U=%d M=%d I=%d O=%d Z=%d ===> %llu\n",
			instructNumber,unmaps,maps,ins,outs,zeros,totalCost);
	}
}

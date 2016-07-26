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
   		pager = new Aging(frame_table,page_table,nf,rand,physical);
   	}

	ifstream infile (inputFileName.c_str());
	string str;

		while (getline(infile, str)) {
	    istringstream iss(str);
	    int write;
	    int pageNum;
	 	pager->referenceCount++;
	 	
	    if (!(iss >> write >>pageNum)) {
	    	cout<<"iss is false in if statement, iss fail? "<<iss.fail()<<"iss good? "<<iss.good()<<", eof? "<<iss.eof()<<endl;
	    	break;
	    }
		
	    if (o){
	    cout<<"==> inst: "<<write<<" "<<pageNum<<endl;
		}

		//cout<<"referenceCount: "<<pager->referenceCount<<endl;
	 	
		Page *oldPage;
	    if (page_table[pageNum].PRESENT == 0){
	    	//cout<<"not present"<<endl;
	    	int frameIndex = pager->get_frame();
	    	int oldPageNum =  frame_table[frameIndex].pageNum;

	    	//cout<<"frame index: "<<frameIndex<<", page num: "<<pageNum<<endl;
	    	oldPage = (frame_table[frameIndex].page);
	    	if (frame_table[frameIndex].pageNum>=0){//-1 will be set before the first reference
	    	//cout<<"page num not -1"<<endl;
	    	if (o){
	    	printf("%d: UNMAP   %2d %2d\n",instructNumber,frame_table[frameIndex].pageNum,oldPage->frame);}
			pager->unmap(oldPage);//unmap previous virtual page associated with selected frame
			unmaps++;
	    	}
	    	if (oldPage->MODIFIED == 1){
	    		pager->page_out(oldPage);//page out if dirty
	    		outs++;
	    		if (o){
	    		printf("%d: OUT     %2d %2d\n",instructNumber,oldPageNum,frameIndex);
	    		}
	    	}

	    	if (page_table[pageNum].PAGEDOUT ==1){
	    		//cout<<"paged out"<<endl;
	    		if (o) {printf("%d: IN      %2d %2d\n",instructNumber,pageNum,frameIndex);}
	    		pager->page_in(&(page_table[pageNum])); //page in if paged out
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

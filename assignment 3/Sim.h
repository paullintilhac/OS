#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Tables.h"
#include "Pager.h"
using namespace std;
class Sim{
public:
	Sim(string inputFileName,const int nf);
	Page page_table[64];
	Frame* frame_table;
	int nFrames;
	Pager pager;
};

Sim::Sim(string inputFileName,int nf)
	: nFrames{nf}
    {
   	frame_table= new Frame[nf];
	pager = LRU(frame_table);
	cout<<"inside constructor"<<endl;
	ifstream infile (inputFileName.c_str());
	string str;
		while (getline(infile, str)) {
	    istringstream iss(str);
	    cout<<"string: "<<str<<endl;

	    bool write;
	    int pageNum;
	    if (!(iss >> write >>pageNum)) { 
	    	cout<<write<<" " <<pageNum<<endl; 
	    	break;
	    } 
	    if (page_table[pageNum].PRESENT == 0){
	    	int frameIndex = pager.get_frame();
	    	page_table[pageNum].frame = frameIndex;
	    	if (page_table[pageNum].PAGEDOUT ==1){
	    		pager.page_in(frame_table[frameIndex]);
	    	}
	    	else{
	    		pager.zero(frame_table[frameIndex]);
	    	}
	    	pager.map(page_table[pageNum],frame_table[frameIndex]);
	    }
	    pager.update_pte(page_table[pageNum]);
	}
}

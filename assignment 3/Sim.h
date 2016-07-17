#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Tables.h"
#include "Pager.h"
using namespace std;
class Sim{
public:
	Sim(string inputFileName,int nf);
	Page page_table[64];
	Frame* frame_table;
	int nFrames;
	Pager* pager;
	static int instructNumber;
};

Sim::Sim(string inputFileName,int nf){
    nFrames = nf;

   	frame_table= new Frame[nf];
	pager = new LRU(frame_table);
	
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
	    cout<<"==> inst: "<<write<<" "<<pageNum<<endl;

	    if (page_table[pageNum].PRESENT == 0){
	    	Page oldPage = *(frame_table[frameIndex].page
	    	int frameIndex = pager->get_frame();
	    	pager->unmap(oldPage);//unmap previous virtual page associated with selected frame
	    	if (oldPage.MODIFIED == 1){
	    		pager->page_out(frame_table[frameIndex]);
	    	}
	    	if (page_table[pageNum].PAGEDOUT ==1){
	    		cout<<Sim.instructNumber<<": IN "<<frameIndex<<" "<<pageNum<<endl;
	    		pager->page_in(frame_table[frameIndex]);
	    	}
	    	else{	    	
	    		cout<<Sim.instructNumber<<": ZERO "<<pageNum<<endl;
	    		pager->zero(frame_table[frameIndex]);
	    	}
	    	cout<<Sim.instructNumber<<": MAP "<<frameIndex<<" "<<pageNum<<endl;
	    	pager->map(page_table[pageNum],frameIndex);
	    }
	    pager->update_pte(page_table[pageNum],write);
	}
}

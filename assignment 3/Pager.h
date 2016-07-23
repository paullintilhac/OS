#ifndef PAGER_H
#define PAGER_H
#include <list>
#include <bitset>
#include "Tables.h"
#include "Rand.h"
typedef list<Frame*> FrameList;
typedef list<int> IntList;

class Pager{
public:
	Pager(Frame* ft, const int size,Rand* r)
	:  classes(size,r)
	{
		frame_table=ft;
		nFrames = size;
		for (int i =0;i<size;++i){
	    	freeList.push_back(&(ft[i]));
	    	fifoList.push_back(i);
	    }
	    referenceCount =0;
	    int startIndex= 0;
	    i=fifoList.begin();
	    this->rand = r;
	};

	int get_frame();
	int allocate_frame_from_free_list();
	page_in(Frame frame);
	page_out(Frame frame);
	zero(Frame frame);
	map(Page *page, int frameIndex, int pageNum);
	unmap(Page *page);
	FrameList freeList;
	list<int> fifoList;
	Frame* frame_table;
	Class classes;
	Rand* rand;
	int referenceCount;
	int nFrames;
	list<int>::iterator i;

	virtual int update_pte(Page *page, int write){
		referenceCount++;
		page->referenceCount = referenceCount;
		if (!write){
			page->REFERENCED=1;
		} else {
			page->REFERENCED=1;
			page->MODIFIED=1;
		}
	}
	virtual int allocate_frame(){
	//do nothing
	};

};

class LRU : public Pager {
public:
	LRU(Frame* ft,int size,Rand* rand)
	: Pager(ft, size, rand){};
	int allocate_frame(){
		//cout<<"inside allocate frame"<<endl;
		int minCount = frame_table[0].page->referenceCount;
		int lruFrameIndex=0;
		for (int i=0;i<nFrames;++i){
			//cout<<"i: "<<i<<", minCount: "<<minCount<<", lruFrameIndex: "<<lruFrameIndex<<endl;
			if (frame_table[i].page->referenceCount<minCount){
				lruFrameIndex = i;
				minCount = frame_table[i].page->referenceCount;
			}
		}
		return lruFrameIndex;
	}
};

class NRU : public Pager {
public: 
	NRU(Frame* ft, Page* pt,int size, Rand* rand)
	: Pager(ft, size, rand){
		page_table =pt; 
	};
	Page* page_table;
	int allocate_frame(){
		
		int pageNumber = classes.get_page_number();
		int frameNumber = page_table[pageNumber].frame;
		return frameNumber;
	}
};

class Random : public Pager {
public:
	Random(Frame* ft, int size, Rand* rand)
	: Pager(ft,size,rand){};
	int allocate_frame(){
		int frameNumber = rand->myrandom(nFrames);
		return(frameNumber);
	}
};

class FIFO : public Pager {
public:
	FIFO(Frame* ft, int size, Rand* rand)
	: Pager(ft,size,rand){};
	int allocate_frame(){
		int frameNumber = fifoList.front();
		fifoList.pop_front();
		fifoList.push_back(frameNumber);
		return(frameNumber);
	}
};

class Clock : public Pager {
public:
	Clock(Frame* ft,Page* pt, int size, Rand* rand,bool phys)
	: Pager(ft, size, rand){
		page_table = pt;
		physical = phys;
	};
	Page* page_table;
	bool physical;
	int allocate_frame(){
		//cout<<"clock pointer: "<<*i<<endl;
		int count = 0;
		while (frame_table[(*i)].page->REFERENCED==1){
			//cout<<"count: "<<++count<<", frameIndex: "<<*i<<endl;
			frame_table[(*i)].page->REFERENCED=0;
			i++;
			if (i==fifoList.end()){
				i=fifoList.begin();
			}
		}
		//cout<<"frameIndex to be replaced: "<<*i<<endl;
		int frameNumber = (*i);
		i++;
		if (i==fifoList.end()){
			i=fifoList.begin();
		}
		if (!physical){
			frameNumber = page_table[frameNumber].frame;
		}
		//cout<<"frameindex after advancing: "<<*i<<endl;
		return frameNumber;
	};

	virtual update_pte(Page *page, int write){ 
		referenceCount++;
		page->referenceCount = referenceCount;
		if (referenceCount==9){
	    	for (int i=0; i<(sizeof(page_table)/sizeof(page_table[0]));++i){
	    		page_table[i].REFERENCED =0;
	    		classes.update(page_table[i]);
	    	}
		}
		if (!write){
			page->REFERENCED=1;
		} else {
			page->REFERENCED=1;
		
			page->MODIFIED=1;
		}	
	}
};

class SC : public Pager {
public: 
	SC(Frame* ft, int size, Rand* rand)
	: Pager(ft,size,rand){};
	int allocate_frame(){
		int count = 0;
		while (frame_table[(*i)].page->REFERENCED==1){
			//cout<<"count: "<<++count<<", frameIndex: "<<*i<<endl;
			frame_table[(*i)].page->REFERENCED=0;
			i++;
			if (i==fifoList.end()){
				i=fifoList.begin();
			}
		}
		//cout<<"frameIndex to be replaced: "<<*i<<endl;
		int frameNumber = (*i);
		i++;
		if (i==fifoList.end()){
			i=fifoList.begin();
		}
		return frameNumber;

	}		

};

class Aging : public Pager {
public:
	Aging(Frame* ft,Page* pt,int size, Rand* rand)
	: Pager(ft,size,rand){
		page_table = pt;
		bitCounter = 0;
		cout<<"bitset initial values: "<<bitCounter<<endl;
	};
	bitset<32> bitCounter;
	Page* page_table;
	int update_pte(Page *page, int write){
		referenceCount++;
		page->referenceCount = referenceCount;
		if (!write){
			page->REFERENCED=1;
		} else {
			page->REFERENCED=1;
			page->MODIFIED=1;
		}
	}
};


int Pager::allocate_frame_from_free_list(){

	int frameIndex;

	if (freeList.size()>0){
	frameIndex = nFrames-freeList.size();
	freeList.pop_front();
	}	
	else{
	//cout<<"free list empty, returning -1"<<endl;
	frameIndex = -1;
	}
	return(frameIndex);
}

int Pager::get_frame(){
	int frameIndex;
	frameIndex = allocate_frame_from_free_list();
	if (frameIndex == -1){
		frameIndex =allocate_frame();
	}
	return frameIndex;
}


int Pager::map(Page *pte,int frameIndex, int pageIndex){
	pte->frame = frameIndex;
	pte->PRESENT = 1;
	frame_table[frameIndex].pageNum = pageIndex;
	frame_table[frameIndex].page = pte;
	//frame_table[frameIndex].locked = false;
}

int Pager::unmap(Page *pte){
	frame_table[pte->frame].pageNum = -1;
	frame_table[pte->frame].page->PRESENT = 0;
	frame_table[pte->frame].page = 0;
	//frame_table[frameIndex].locked = true;
}

int Pager::zero(Frame frame){
	//do nothing
}

Pager::page_out(Frame frame){
	frame.page->PAGEDOUT=1;
}
 
int Pager::page_in(Frame frame){
	frame.page->PAGEDOUT=0;
	frame.page->PRESENT =1;
}




#endif

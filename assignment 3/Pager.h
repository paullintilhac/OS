#ifndef PAGER_H
#define PAGER_H
#include <list>
#include "Tables.h"
#include "Rand.h"
typedef list<Frame*> FrameList;

class Pager{
public:
	Pager(Frame* ft, const int size,Rand* r)
	:  classes(size,rand)
	{
		frame_table=ft;
		nFrames = size;
		for (int i =0;i<size;++i){
	    	freeList.push_back(&(ft[i]));
	    }
	    referenceCount =0;
	    this->rand = r;
	};

	int get_frame();
	int allocate_frame_from_free_list();
	page_in(Frame frame);
	page_out(Frame frame);
	zero(Frame frame);
	map(Page *page, int frameIndex, int pageNum);
	unmap(Page *page);
	update_pte(Page *page, int write);
	FrameList freeList;
	Frame* frame_table;
	Class classes;
	Rand* rand;
	int referenceCount;
	int nFrames;


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
	NRU(Frame* ft, int size, Rand* rand)
	: Pager(ft, size, rand){
	};
	int allocate_frame(){

		int frameNumber = classes.get_frame_number();
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

int Pager::update_pte(Page *page,int write){
	referenceCount++;
	page->referenceCount = referenceCount;
	if (!write){
		page->REFERENCED=1;
	} else {
		page->REFERENCED=1;
		page->MODIFIED=1;
	}
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

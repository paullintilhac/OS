#ifndef PAGER_H
#define PAGER_H
#include <list>
#include "Tables.h"
typedef list<Frame*> FrameList;

class Pager{
public:
	Pager(Frame* ft):frame_table{ft}
	{
		for (int i =0;i<(sizeof(ft)/sizeof(ft[0]));++i){
	    	freeList.push_back(&(ft[i]));
	    }
	};
	int get_frame();
	int allocate_frame_from_free_list();
	void page_in(Frame frame);
	void page_out(Frame frame);
	void zero(Frame frame);
	void map(Frame frame);
	void unmap(Frame frame);
	void update_pte(Frame frame);
	FrameList freeList;
	Frame* frame_table;
	virtual int allocate_frame();
};

class LRU : public Pager{
public:
	int allocate_frame(){
		int minCount = 2^32-1;
		int lruFrameIndex;
		for (int i=0;i<(sizeof(frame_table)/sizeof(frame_table[0]));++i){
			if (frame_table[i].page->referenceCount<minCount){
				lruFrameIndex = i;
				minCount = frame_table[i].page->referenceCount;
			}
		}
		return lruFrameIndex;
	}
};


int Pager::allocate_frame_from_free_list(){
	int frameIndex;
	if (freeList.size()>0){
	freeList.pop_front();
	frameIndex = (sizeof(frame_table)/sizeof(frame_table[0]))-freeList.size();
	}	
	else{
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

Pager::update_pte(Frame frame,bool write){
	if (!write){
		frame.page->REFERENCED=1;
	} else{
		frame.page->MODIFIED=1;
	}
}

Pager::map(Page pte,int frameIndex){
	pte.frame = frameIndex;
	pte.PRESENT = 1;
}

Pager::zero(Frame frame){
	frame.page->PRESENT=0;
	frame.page->PAGEDOUT=0;
}

Pager::page_out(Frame frame){
	frame.page->PAGEDOUT=1;
	frame.page->PRESENT=0;
}

Pager::page_in(Frame frame){
	frame.page->PAGEDOUT=0;
	frame.page->PRESENT =1;
}




#endif

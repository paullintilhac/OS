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
	    this->rand = r;
	};

	int get_frame();
	int allocate_frame_from_free_list();
	page_in(Page *page);
	page_out(Page *page);
	zero(Frame frame);
	map(Page *page, int frameIndex, int pageNum);
	unmap(Page *page);
	FrameList freeList;
	list<int> fifoList;
	Frame* frame_table;
	Class classes;
	Rand* rand;
	unsigned int *referenceCounters;
	int referenceCount;
	int nFrames;

	virtual int update_pte(Page *page, int write){

		if (!write){
			page->REFERENCED=1;
		} else {
			page->REFERENCED=1;
			page->MODIFIED=1;
		}
	}
	virtual int allocate_frame(){
	//do nothing
	}
	virtual int touch(Page* page){
		//do nothing
	}

};

class LRU : public Pager {
public:
	LRU(Frame* ft,int size,Rand* rand)
	: Pager(ft, size, rand){
		referenceCounters = new unsigned int[size];
		for (int i=0; i<size;i++){
			referenceCounters[i]=0;
		}
	};
	int allocate_frame(){
		//cout<<"inside allocate frame"<<endl;
		int minCount = referenceCounters[0];
		int lruFrameIndex=0;
		for (int i=0;i<nFrames;++i){
			//cout<<"counter: "<<referenceCounters[i]<<", minCount: "<<minCount<<", lruFrameIndex: "<<lruFrameIndex<<endl;
			if (referenceCounters[i]<minCount){
				lruFrameIndex = i;
				minCount = referenceCounters[i];
			}
		}
		return lruFrameIndex;
	}
	virtual int update_pte(Page *page, int write){
		referenceCount++;
		referenceCounters[page->frame] = referenceCount;
		if (!write){
			page->REFERENCED=1;
		} else {
			page->REFERENCED=1;
			page->MODIFIED=1;
		}
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
		
		int idx = 0;
		int frameNumber =0;
		int pNum;

		//updates classification for all valid pages 
		for (int i=0;i<64;++i){
			if (page_table[i].PRESENT==1){
		    		classes.update(&(frame_table[page_table[i].frame]));	
		    		//cout<<"page: "<<frame_table[page_table[i].frame].pageNum<<", class: "<<frame_table[page_table[i].frame].classNumber<<endl;
	    	}
	    	if (referenceCount%10==9){
		    			//cout<<"resetting reference bit for page "<<i<<endl;
		    	page_table[i].REFERENCED =0;
		    }

	    }

	    //randomly selects an page index in the lowest non-empty class
	    int pageNumber = classes.get_page_number();

	    //this piece of code steps through each valid page table entry with the correct class bit set until it finds the pageNumer-th page in that subset
	   	for (int i=0;i<64;++i){

			if (page_table[i].PRESENT ==1 && classes.classVec[page_table[i].frame][classes.lowestClass]==true){
				
				if (idx==pageNumber){
					pNum = i;
					frameNumber = page_table[i].frame;
					break;
				}
				idx++;
			}


		}
		//cout<<"randval: "<<pageNumber<<", selected page: "<<pNum<<", selected frame: "<<frameNumber<<endl;

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
		int listSize;
		if (physical){
			listSize = size;
		} else{
			listSize = 64;
		}
		for (int i=0;i<listSize;++i){
			clockList.push_back(i);
		}
		i = clockList.begin();
	};
	Page* page_table;
	list<int>::iterator i;
	list<int> clockList;

	bool physical;
	int allocate_frame(){
		//cout<<"clock pointer: "<<*i<<endl;
		int count = 0;
		if (physical){
		while (frame_table[(*i)].page->REFERENCED==1){
			//cout<<"count: "<<++count<<", frameIndex: "<<*i<<endl;
			frame_table[(*i)].page->REFERENCED=0;
			i++;
			if (i==clockList.end()){
				i=clockList.begin();
			}
		}
		} else{
			while (page_table[(*i)].REFERENCED==1|page_table[(*i)].PRESENT==0){
			//cout<<"count: "<<++count<<", frameIndex: "<<*i<<endl;
			page_table[(*i)].REFERENCED=0;
			i++;
			if (i==clockList.end()){
				i=clockList.begin();
			}
		}
		}
		//cout<<"frameIndex to be replaced: "<<*i<<endl;
		int frameNumber = (*i);
		i++;
		if (i==clockList.end()){
			i=clockList.begin();
		}
		if (!physical){
			frameNumber = page_table[frameNumber].frame;
		}
		//cout<<"frameindex after advancing: "<<*i<<endl;
		return frameNumber;
	};

	virtual update_pte(Page *page, int write){ 
		referenceCount++;
		
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
	: Pager(ft,size,rand){
		for (int i=0;i<size;++i){
			clockList.push_back(i);
		}
		i = clockList.begin();
	};
	list<int>::iterator i;
	list<int> clockList;
	int allocate_frame(){
		int count = 0;
		while (frame_table[(*i)].page->REFERENCED==1){
			//cout<<"count: "<<++count<<", frameIndex: "<<*i<<endl;
			frame_table[(*i)].page->REFERENCED=0;
			i++;
			if (i==clockList.end()){
				i=clockList.begin();
			}
		}
		//cout<<"frameIndex to be replaced: "<<*i<<endl;
		int frameNumber = (*i);
		i++;
		if (i==clockList.end()){
			i=clockList.begin();
		}
		return frameNumber;

	}		

};

class Aging : public Pager {
public:
	Aging(Frame* ft,Page* pt,int size, Rand* rand,bool p)
	: Pager(ft,size,rand){
		int counterSize;
		physical = p;
		if (physical){
			counterSize = size;
		} else{
			counterSize = 64;
		}
		referenceCounters = new bitset<32>[counterSize];
			for (int i=0;i<counterSize;++i)
			{
				referenceCounters[i]=0;
				//cout<<"referenceCounters: "<<referenceCounters[i]<<endl;
			}
		page_table = pt;
	};
	bool physical;
	Page* page_table;
	bitset<32> *referenceCounters;
	int update_pte(Page *page, int write){
		referenceCount++;

		if (!write){
			page->REFERENCED=1;
		} else {
			page->REFERENCED=1;
			page->MODIFIED=1;
		}
	}

	int allocate_frame(){
		int size;
		if (physical){
			size = nFrames;
		} else{
			size = 64;
		}

		for (int i=0;i<size;++i){

			if (physical){
				referenceCounters[i]>>=1;
				if (frame_table[i].page->REFERENCED){
					referenceCounters[i].set(31,1);
					frame_table[i].page->REFERENCED = 0;
				}
			} else{
				//cout<<"i: "<<i<<", counter before: "<<referenceCounters[i]<<endl;
				referenceCounters[i]>>=1;
				if (page_table[i].PRESENT==1){
				if (page_table[i].REFERENCED){
					referenceCounters[i].set(31,1);
					//cout<<"counter after: "<<referenceCounters[i]<<endl;
					page_table[i].REFERENCED = 0;
				}
				}
			}
		}
		unsigned int minCount;
		if (physical){
			 minCount= (referenceCounters[0].to_ulong());
		} else{
			minCount = (referenceCounters[frame_table[0].pageNum].to_ulong());
		}
		int lruFrameIndex=0;
		for (int i=0;i<size;++i){
			//cout<<"i: "<<i<<endl;
			if (!physical && page_table[i].PRESENT==0){
				continue;
			}
			//cout<<"actually processing "<<i<<endl;
			unsigned int refInt = (referenceCounters[i].to_ulong());
			if (refInt<minCount){
				lruFrameIndex = i;
				minCount = refInt;
			}
		}
		if (!physical){
			lruFrameIndex = page_table[lruFrameIndex].frame;
		}
		return lruFrameIndex;
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
	//cout<<"setting frame "<<frameIndex<<" page number to "<<frame_table[frameIndex].pageNum<<endl;
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

Pager::page_out(Page *page){
	//cout<<"page number: "<<frame.pageNum<<endl;
	page->PAGEDOUT=1;
}
 
int Pager::page_in(Page *page){
	//page->PAGEDOUT=0;
	page->PRESENT =1;
	page->MODIFIED=0;
}




#endif

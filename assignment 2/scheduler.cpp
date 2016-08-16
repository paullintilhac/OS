#include "Scheduler.h"
#include <string>
#include <iostream>
using namespace std;

Scheduler::add_process(Process *p){
    this->readyQueue.push_back(p);
}

Scheduler::mapProcess(Process* p){
		BitMap* bm;
		if (p->dynamic_priority==-1){
			bm = &(this->expired);
			p->dynamic_priority = p->static_priority-1;
		} else{
			bm = &(this->active);
			//if (p->state == "BLOCK"){
			//	p->dynamic_priority=p->static_priority-1;
			//}
		}

		if (bm->bitmap[p->dynamic_priority].flag==false){
			bm->bitmap[p->dynamic_priority].flag=true;
		}
		
		bm->bitmap[p->dynamic_priority].pList.push_back(p);
		
	}


Scheduler::unmapProcess(Process* p){
		
		this->active.bitmap[p->dynamic_priority].pList.pop_front();
		//cout<<"after popping front"<<endl;
		if (this->active.bitmap[p->dynamic_priority].pList.size()==0 && this->active.bitmap[p->dynamic_priority].flag==true){
			this->active.bitmap[p->dynamic_priority].flag=false;
		}

	}

Scheduler::get_next_process(){
	cout<<"ERROR: no specific scheduler constructed -- need to override getProcess()"<<endl;
	//exit(1);
}

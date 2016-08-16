#ifndef SCHEDULER_H
#define SCHEDULER_H
#include "Process.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <typeinfo>
#include <list>
using namespace std;
typedef list<Process*> ProcessList;

class Bit{
public:
	bool flag;
	ProcessList pList;
	Bit(){
		flag = false;
	}
};

class BitMap{
public:
	Bit bitmap[4];
};

class Scheduler{
public:
	int quantum;
	virtual add_process(Process *p);
	unmapProcess(Process *p);
	mapProcess(Process *p);
	virtual get_next_process();
	string schedulerAlgo;
	int numExpired;
	Process *CURRENT_PROCESS;
	list<Process*> readyQueue;
	BitMap active;
	BitMap expired;
};

class FCFS : public Scheduler{
	public:
    FCFS(){
    	this->quantum = 100000000;
    	this->schedulerAlgo = "FCFS";
    }
	get_next_process(){
 	if (this->readyQueue.size()>0){
	this->CURRENT_PROCESS = this->readyQueue.front();
	this->readyQueue.pop_front();
	} else{
		this->CURRENT_PROCESS = 0;
	}
	}
};

class LCFS : public Scheduler{
	public:
	LCFS(){
    	this->quantum = 100000000;
    	this->schedulerAlgo = "LCFS";
    }
	get_next_process(){
	//first come, first served
	if (this->readyQueue.size()>0){
	this->CURRENT_PROCESS = this->readyQueue.back();
	this->readyQueue.pop_back();
	} else{
		this->CURRENT_PROCESS = 0;
	}
	}
};

class SJF : public Scheduler{
	public:
	SJF(){
		this->quantum =100000000;
    	this->schedulerAlgo = "SJF";
	}
	get_next_process(){
	//first come, first served
	if (this->readyQueue.size()>0){
	int shortest = 10000000;
	ProcessList::iterator shortestProcess;
	for (ProcessList::iterator p = this->readyQueue.begin();p != this->readyQueue.end();++p){
		if ((*p)->remainingExecTime<shortest){
			shortest = (*p)->remainingExecTime;
			shortestProcess = p;
		}
	}
	this->CURRENT_PROCESS = *shortestProcess;
	this->readyQueue.erase(shortestProcess);

	} else{
		this->CURRENT_PROCESS = 0;
	}
	}
};

class RR : public Scheduler{
	public:
	RR(int q){
		this->quantum =q;
		stringstream s;
		s <<"RR "<<q;
		this->schedulerAlgo = s.str();
	}
	get_next_process(){
		if (this->readyQueue.size()>0){
		this->CURRENT_PROCESS = this->readyQueue.front();
		this->readyQueue.pop_front();
		} else{
			this->CURRENT_PROCESS = 0;
		}
	}
};

class PRIO : public Scheduler{
	public:
	PRIO(int q){
		this->quantum =q;
		stringstream s;
		s<<"PRIO "<<q;
    	this->schedulerAlgo = s.str();
    	numExpired = 0;
	}
	
	add_process(Process* p){
		this->mapProcess(p);
	}

	get_next_process(){
		
			int maxPriority=-1;
			for(int i=0;i<4;i++){
				if (active.bitmap[i].flag ==true){
					maxPriority=i;
				}
			}

			if (maxPriority==-1){
				swap(active, expired);
				maxPriority=-1;
				for(int i=0;i<4;i++){
				if (active.bitmap[i].flag ==true){
					maxPriority=i;
				}
			}
			}

			//cout<<"max priority: "<<maxPriority<<endl;
		    if (maxPriority ==-1){
		    	CURRENT_PROCESS = 0;
		    }
		    else{
		    	CURRENT_PROCESS = this->active.bitmap[maxPriority].pList.front();
		    	unmapProcess(CURRENT_PROCESS);
		    	
		    }
		
		}
};


#endif
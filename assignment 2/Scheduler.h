#ifndef SCHEDULER_H
#define SCHEDULER_H
#include "Process.h"
#include <list>
#include <string>
#include <iostream>
using namespace std;
typedef list<Process*> ProcessList;

class Scheduler{
public:
	int quantum;
	add_process(Process *p);
	virtual get_next_process();
	string schedulerAlgo;
	Process *CURRENT_PROCESS;
	list<Process*> readyQueue;

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
		this->schedulerAlgo = "RR";
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
    	this->schedulerAlgo = "PRIO";
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
#endif
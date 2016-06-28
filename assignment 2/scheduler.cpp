#include "Scheduler.h"
#include <string>
#include <iostream>
using namespace std;

Scheduler::Scheduler(string type){
	this->schedulerAlgo = type;
}

Scheduler::add_process(Process *p){
    this->readyQueue.push(p);
}

Scheduler::get_next_process(){
	//first come, first served
	if (this->readyQueue.size()>0){
	this->CURRENT_PROCESS = this->readyQueue.front();
	//cout<<"process selected by scheduler: "<<this->CURRENT_PROCESS<<endl;
	this->readyQueue.pop();
	} else{
		//cout<<"READY QUEUE IS EMPTY, RETURNING 0 POINTER..."<<endl;
		this->CURRENT_PROCESS = 0;
	}
}
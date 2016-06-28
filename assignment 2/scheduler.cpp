#include "Scheduler.h"
#include <string>
using namespace std;
Scheduler::Scheduler(string type){
	this->schedulerAlgo = type;
}

Scheduler::add_process(Process *p){
    this->readyQueue.push(p);
}

Scheduler::get_next_process(){
	//first come, first served
	this->CURRENT_PROCESS = this->readyQueue.front();
	this->readyQueue.pop();
}
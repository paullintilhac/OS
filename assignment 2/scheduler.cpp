#include "Scheduler.h"
#include <string>
#include <iostream>
using namespace std;



Scheduler::add_process(Process *p){
    this->readyQueue.push_back(p);
}

Scheduler::get_next_process(){
	cout<<"ERROR: no specific scheduler constructed -- need to override getProcess()"<<endl;
	//exit(1);
}

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "Util.h"
#include "Sched.h"
using namespace std;



class Sim{
public:
	Sim(string inputFileName,string algoName);
	run_simulation();
	insert_event(IOEvent e);
	Sched* sched;
	int CURRENT_TIME;
	bool CALL_SCHEDULER;
	int PREV_TRACK;
	IOProcess* CURRENT_RUNNING_PROCESS;
};

Sim::Sim(string inputFileName,string algoName){
   	if (algoName=="i"){
   	   	sched= new FIFO(inputFileName);
   	}
   	if (algoName =="j"){
   		sched = new SSTF(inputFileName);
   	}
   	if (algoName == "s"){
   		sched = new SCAN(inputFileName);
   	}
   	if (algoName == "c"){
   		sched = new CSCAN(inputFileName);
   	}
   	if (algoName == "f"){
   		sched = new FSCAN(inputFileName);
   	}
   	CALL_SCHEDULER = false;
   	PREV_TRACK=0;
   	CURRENT_RUNNING_PROCESS = NULL;
}


Sim::run_simulation(){
	int count = 0;
	int tot_movement=0;
	int max_waittime=0;
	float total_turnaround=0;
	float avg_turnaround=0;
	float total_waittime=0;
	float avg_waittime=0;
	for (IOEventList::iterator i = sched->ioQueue.begin();i!=sched->ioQueue.end();++i){
		count++;
		CURRENT_TIME = i->timestamp;

		IOProcess* p = i->process;
		//cout<<"event info: timestamp: "<<i->timestamp<<", old state: "<<i->oldState<<", new state: "<<i->newState<<endl;
		//cout<<"state_ts: "<<p->state_ts<<endl;
		p->time_in_prev_state = CURRENT_TIME-p->state_ts;
		//cout<<"time in prev state: "<<p->time_in_prev_state<<endl; 
		p->state_ts = CURRENT_TIME;

    	if (i->newState =="READY"){
    	sched->add_process(p);
        CALL_SCHEDULER = true;
		}
		if (i->newState == "SEEK"){
			int waittime = CURRENT_TIME-p->startTS;
			total_waittime+=waittime;
			if (waittime>max_waittime){
				max_waittime = waittime;
			}
			tot_movement+=abs(p->track-PREV_TRACK);
			IOEvent finishEvent(CURRENT_TIME+abs(p->track-PREV_TRACK),p,"SEEK","DONE");
          	this->insert_event(finishEvent);
		}

		if (i->newState == "DONE"){
			PREV_TRACK = p->track;
			total_turnaround+=CURRENT_TIME-p->startTS;
			CALL_SCHEDULER=true;
			CURRENT_RUNNING_PROCESS=NULL;
			//do nothing
		}

 		IOEventList::iterator iForward = i;
    	iForward++;
    	int nextTime = iForward->timestamp;

    	if (CALL_SCHEDULER){
	    	if (nextTime==i->timestamp&&iForward != sched->ioQueue.end()){
	        //cout<<"encountered the same time stamp, advancing forward..."<<endl;
	        continue;
	      	}
	      	CALL_SCHEDULER = false;
	      	if (CURRENT_RUNNING_PROCESS==NULL){
	      	   CURRENT_RUNNING_PROCESS=sched->get_next_process();

	           if (CURRENT_RUNNING_PROCESS == NULL){
	            //downTime+=nextTime-CURRENT_TIME;
	            continue;
	           }
	           IOEvent seekEvent(i->timestamp,CURRENT_RUNNING_PROCESS,"READY","SEEK");
	           this->insert_event(seekEvent);
	           
	      	}
    	}
		//cout<<"PREV_TRACK: "<<PREV_TRACK<<endl;
	}
	avg_turnaround = total_turnaround/sched->ioProcesses.size();
	avg_waittime = total_waittime/sched->ioProcesses.size();
	printf("SUM: %d %d %.2lf %.2lf %d\n",
		CURRENT_TIME,
		tot_movement,
		avg_turnaround,
		avg_waittime,
		max_waittime);
}

Sim::insert_event(IOEvent e){

	IOEventList::iterator inserter = sched->ioQueue.begin();

 	while (++inserter != sched->ioQueue.end() && inserter->timestamp<= e.timestamp){
    //do nothing else
 	};

  
	sched->ioQueue.insert(inserter, e);
  
}
#include "DES.h"
#include "Process.h"
#include "Event.h"
#include "Rand.h"
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;
DES::DES(string originFile, Rand* r,Scheduler* s){
	ifstream infile (originFile.c_str());
	string str;
  int pCount=0;
  cout<<"about to go through while loop"<<endl;
	while (getline(infile, str)) {
	     
	    istringstream iss(str);

	    int AT,TC,CB,IO;
	    if (!(iss >> AT >> TC>>CB>>IO)) { 
	    	//cout<<"breaking out of while loop \n"; 
	    	break;
	    } 
      int myRand = r->myrandom(4);
      //cout<<"myrand: "<<myRand<<endl;
     
		  Process *p=new Process(AT,TC,CB,IO,myRand,pCount);
      stringstream ss;
      ss<<AT<<" "<<pCount<<" "<<p->timeInPrevState<<": CREATED -> READY"<<endl;
      this->verboseSummary<<ss.str();

      pCount++;
		  Event evt(AT,p,"CREATED","READY");
	    this->events.push_back(evt);
      
	    //cout<<"AT: "<<AT<<", TC: "<<TC<<", CB: "<<CB<<", IO: "<<IO<<endl;

	}

    this->rand = r;
    this->sched=s;
    infile.close();

}

DES::insert_event(Event e){

	EventList::iterator inserter = this->events.begin();

 	while (++inserter != this->events.end() && inserter->timestamp<= e.timestamp){
    //do nothing else
 	};

  
	this->events.insert(inserter, e);
  
}

DES::run_simulation(){
int count = 0;
Process* CURRENT_RUNNING_PROCESS =0;
for (EventList::iterator i=this->events.begin();i != this->events.end();++i){
    Process* p = i->process;
    this->CURRENT_TIME = i->timestamp;
    i->process->timeInPrevState = CURRENT_TIME-i->process->state_ts;
    i->process->state_ts = this->CURRENT_TIME;
    if (i->oldState!="CREATED")
    this->verboseSummary<<CURRENT_TIME<<" "<<p->pCount<<" "<<p->timeInPrevState<<": "<< i->oldState<<" -> "<<i->newState;

    
       //cout<<endl;
        count++;
        //cout<<"count: "<<(count)<<", old state: "<<i->oldState<<", new state: "<<i->newState<<", process pointer: "<<i->process<<endl;
    if (i->oldState == "READY"){
      i->process->totalWaitTime+=i->process->timeInPrevState;
    }

    if (i->oldState == "RUNNG"){
      CURRENT_RUNNING_PROCESS=0;
      sched->CURRENT_PROCESS = 0;
      i->process->remainingExecTime -= i->process->timeInPrevState;
      if (i->process->remainingExecTime == 0){
        stringstream ss;
        ss << setw(4) << setfill('0') << i->process->pCount;
        string s = ss.str();        
        //cout<<"creating process summary"<<endl;
        stringstream summaryStream;
        summaryStream<<s<<" "<<i->process->AT<<" "<<i->process->TC<<" "<<i->process->CB<<" "<<i->process->IO<<" "<<i->process->static_priority<<" |  "
            <<CURRENT_TIME<<" "<<CURRENT_TIME-i->process->AT<<" "<<i->process->totalIOTime<<" "<<i->process->totalWaitTime<<endl;
        //cout<<"summary string being created: "<<summaryStream.str()<<endl;
        this->processSummary<<summaryStream.str();
      }
      //cout<<"resetting current running process to "<<CURRENT_RUNNING_PROCESS<<endl;
    }

    if (i->oldState=="BLOCK"){
      i->process->totalIOTime+=i->process->timeInPrevState;
    }
      
    /*
     for (EventList::iterator j=this->events.begin();j != this->events.end();++j){
       cout<<"before deletion event old state: "<< j->oldState<<", event new state: "<<j->newState<<endl;
       }
    */

    if (count!=1){
    EventList::iterator iLookback = i;
    iLookback--;
    this->events.erase(iLookback);
    }

    /*
    cout<<endl;
      for (EventList::iterator j=this->events.begin();j != this->events.end();++j){
       cout<<"after deletion event old state: "<< j->oldState<<", event new state: "<<j->newState<<endl;
       }
    */

    
    //going from created to ready
    if (i->newState =="READY" && i->process->remainingExecTime>0){
        //cout<<"adding process to run queue"<<endl;
        sched->add_process(i->process);
        CALL_SCHEDULER=true;
        if (i->oldState!="CREATED")
        this->verboseSummary<<endl;
    }

    //going from ready to run
    if (i->newState == "RUNNG"){
        int myRand = rand->myrandom(i->process->CB);
        if (i->process->remainingExecTime<myRand)
          myRand = i->process->remainingExecTime;
        Event runEvent(CURRENT_TIME+myRand,i->process,"RUNNG","BLOCK");
        this->insert_event(runEvent);
        //cout<<"inserting run event"<<endl;
        this->verboseSummary<<" cb="<<myRand<<" rem="<<p->remainingExecTime<<" prio="<<p->dynamic_priority<<endl;
        //cout<<"CB will last for: "<<myRand<<endl;
        //create event for either preemption or blocking
    }

    if (i->newState == "BLOCK"){
        if (i->process->remainingExecTime==0){
          //cout<<"execution finished, no new process created"<<endl;
        } else {
          int myRand = rand->myrandom(i->process->IO);
          //cout<<"IO will last for: "<<myRand<<endl;
          Event blockEvent(CURRENT_TIME+myRand,i->process,"BLOCK","READY");
          this->insert_event(blockEvent);
          this->verboseSummary<<" ib="<<myRand<<" rem="<<p->remainingExecTime<<endl;

        }
        //cout<<"inserting block event"<<endl;

        //create an event for when the process becomes ready again
        CALL_SCHEDULER =true;
    }

    EventList::iterator iForward = i;
    iForward++;

    
    int nextTime = iForward->timestamp;
    
      if (CALL_SCHEDULER){
      
       if (nextTime==i->timestamp&&iForward != this->events.end()){
        //cout<<"encountered the same time stamp, advancing forward..."<<endl;
        continue;
       }
       CALL_SCHEDULER = false;
       //cout<<"current running process: "<<CURRENT_RUNNING_PROCESS<<endl;

       if (CURRENT_RUNNING_PROCESS==0){
           //cout<<"no process running, getting next process..."<<endl;
           sched->get_next_process();
           CURRENT_RUNNING_PROCESS = sched->CURRENT_PROCESS;
           if (CURRENT_RUNNING_PROCESS == 0){
            continue;
           }
       }

       Event runEvent(i->timestamp,CURRENT_RUNNING_PROCESS,"READY","RUNNG");
       //cout<<"run event old state: "<< runEvent.oldState<<", run event new state: "<<runEvent.newState<<", process pointer: "<<runEvent.process<<endl;
      

       this->insert_event(runEvent);
    }
}     
    cout<<this->verboseSummary.str()<<endl;
    cout<<this->processSummary.str()<<endl;
    //cout<<"last event in event queue- oldState: "<<lastEvent->oldState<<", newState: "<<lastEvent->newState<<", process remaining exec time: "<<lastEvent->process->remainingExecTime <<endl;
}
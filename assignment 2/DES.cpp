#include "DES.h"
#include "Process.h"
#include "Event.h"
#include "Rand.h"
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
using namespace std;
DES::DES(string originFile, Rand* r,Scheduler* s){
	ifstream infile (originFile.c_str());
	string str;
	while (getline(infile, str)) {
	
	    istringstream iss(str);

	    int AT,TC,CB,IO;
	    if (!(iss >> AT >> TC>>CB>>IO)) { 
	    	cout<<"breaking out of while loop \n"; 
	    	break;
	    } 
		  Process *p=new Process(AT,TC,CB,IO);
		  Event evt(AT,p,0,1);
	    this->events.push_back(evt);
    
	    cout<<"AT: "<<AT<<", TC: "<<TC<<", CB: "<<CB<<", IO: "<<IO<<endl;

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

for (EventList::iterator i=this->events.begin();i != this->events.end();++i){

    this->CURRENT_TIME = i->timestamp;
    i->process->timeInPrevState = CURRENT_TIME-i->process->state_ts;
    i->process->state_ts = this->CURRENT_TIME;
       cout<<endl;
        count++;
        cout<<"count: "<<(count)<<", old state: "<<i->oldState<<endl;

    if (i->oldState == 2){
      CURRENT_RUNNING_PROCESS=0;
      sched->CURRENT_PROCESS = 0;
      cout<<"resetting current running process to "<<CURRENT_RUNNING_PROCESS<<endl;
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

    if (count>5)
      break;
    //going from created to ready
    if (i->newState ==1){
        cout<<"adding process to run queue"<<endl;
        sched->add_process(i->process);
        CALL_SCHEDULER=true;
    }

    //going from ready to run
    if (i->newState == 2){
        int myRand = rand->myrandom(i->process->CB);
        Event blockEvent(i->timestamp+myRand,i->process,2,3);
        this->insert_event(blockEvent);
        cout<<"CB will last for: "<<myRand<<endl;
        //create event for either preemption or blocking
    }

    if (i->newState == 3){
        int myRand = rand->myrandom(i->process->IO);
        cout<<"IO will last for: "<<myRand<<endl;
        //create an event for when the process becomes ready again
        CALL_SCHEDULER =true;
    }

    EventList::iterator iForward = i;
    iForward++;

    
    int nextTime = iForward->timestamp;
    int nextNewState = iForward->newState;
    int nextOldState = iForward->oldState;

      if (CALL_SCHEDULER){
      
       if (nextTime==i->timestamp&&iForward != this->events.end()){
        cout<<"encountered the same time stamp, advancing forward..."<<endl;
        continue;
       }
       CALL_SCHEDULER = false;
       cout<<"current running process: "<<CURRENT_RUNNING_PROCESS<<endl;

       if (CURRENT_RUNNING_PROCESS==0){
           sched->get_next_process();
           CURRENT_RUNNING_PROCESS = sched->CURRENT_PROCESS;
           if (CURRENT_RUNNING_PROCESS == 0){
            continue;
           }
       }
       Event runEvent(i->timestamp,i->process,1,2);
       cout<<"run event old state: "<< runEvent.oldState<<", run event new state: "<<runEvent.newState<<endl;
      

       this->insert_event(runEvent);
    }

}
}
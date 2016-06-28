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
		  Event evt(AT,p,CREATED,READY);
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
     for (EventList::iterator j=this->events.begin();j != this->events.end();++j){
        printf("pointer to element before deletion: %d\n",j->timestamp);
       }

    if (count!=0){
    EventList::iterator iLookback = i;
    iLookback--;
    this->events.erase(iLookback);
    }
    cout<<endl;
      for (EventList::iterator j=this->events.begin();j != this->events.end();++j){
        printf("pointer to element after deletion: %d\n",j->timestamp);
       }


    count++;
    if (count>10)
      break;
    //going from created to ready
    if (i->newState ==READY){
        cout<<"adding process to run queue"<<endl;
        sched->add_process(i->process);
        CALL_SCHEDULER=true;
    }

    //going from ready to run
    if (i->newState == RUNNING){
        int myRand = rand->myrandom(i->process->CB);
        Event blockEvent(i->timestamp+myRand,i->process,RUNNING,IO);
        this->insert_event(blockEvent);
        cout<<"random number used for blocking event: "<<myRand<<endl;
        //create event for either preemption or blocking
        CALL_SCHEDULER = true;
    }

    if (i->newState == IO){
        int myRand = rand->myrandom(i->process->IO);
        cout<<"random number used for CPU event: "<<myRand<<endl;
        //create an event for when the process becomes ready again
        CALL_SCHEDULER =true;
    }

    EventList::iterator iForward = i;
    iForward++;

    
    int nextTime = iForward->timestamp;
     
    if (CALL_SCHEDULER){
       cout<<"timestamp: "<<i->timestamp<<", next timestamp: "<< nextTime<<endl;
       if (nextTime==i->timestamp){
        cout<<"encountered the same time stamp, advancing forward..."<<endl;
        continue;
       }
       CALL_SCHEDULER = false;
       sched->get_next_process();
       CURRENT_RUNNING_PROCESS = sched->CURRENT_PROCESS;
       cout<<"current running process: "<<CURRENT_RUNNING_PROCESS<<endl;
       if (CURRENT_RUNNING_PROCESS == 0){
        continue;
       }
       Event runEvent(i->timestamp,i->process,READY,RUNNING);
       printf("pointer to new run event: %p\n", &runEvent);

       this->insert_event(runEvent);

      


    }

    cout<<"count: "<<(count)<<endl;
}
}
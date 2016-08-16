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
	while (getline(infile, str)) {
	    istringstream iss(str);
   
	    int AT,TC,CB,IO;
	    if (!(iss >> AT >> TC>>CB>>IO)) { 
	    	//cout<<"breaking out of while loop \n"; 
	    	break;
	    } 
      if (pCount==0){
        this->START_TIME = AT;
      }

      int myRand = r->myrandom(4);

		  Process *p=new Process(AT,TC,CB,IO,myRand,pCount);
      this->processes.push_back(p);
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

DES::run_simulation(bool verbose){
int count = 0;
int finalTime=0;
int lastTime;
double downTime = START_TIME;
Process* CURRENT_RUNNING_PROCESS =0;
double ioCount=0;
double ioTime=0;
for (EventList::iterator i=this->events.begin();i != this->events.end();++i){
    Process* p = i->process;
    lastTime = CURRENT_TIME;
    this->CURRENT_TIME = i->timestamp;
    p->timeInPrevState = CURRENT_TIME-p->state_ts;
    //verboseSummary<<"count: "<<(count)<<", timestamp: "<<CURRENT_TIME<<" state_ts: "<<p->state_ts<<" , old state: "<<i->oldState<<", new state: "<<i->newState<<", process pointer: "<<p<<endl;
    //cout<<"time in prev state: "<<p->timeInPrevState<<endl;
    p->state_ts = this->CURRENT_TIME;
    

    //verboseSummary<<"ready queue size: "<<sched->readyQueue.size()<<endl;
    //verboseSummary<<"# invocations of randomizer: "<<rand->randCount<<endl;
        //cout<<endl;
        count++;
    if (i->oldState == "READY"){
      p->totalWaitTime+=p->timeInPrevState;
    }
    //cout<<"remaining CB at top: "<<p->remainingCB<<", timeInPrevState: "<<p->timeInPrevState<<endl;

    if (i->oldState == "RUNNG"){
      CURRENT_RUNNING_PROCESS=0;
      sched->CURRENT_PROCESS = 0;
      //verboseSummary<<"decrementing remaining cb: "<<p->remainingCB<<" by tps: "<<p->timeInPrevState<<endl;
      p->remainingExecTime -= p->timeInPrevState;
      p->remainingCB -= p->timeInPrevState;
      //verboseSummary<<"remaining CB: "<<p->remainingCB<<endl;
      if (p->remainingExecTime == 0){
        p->TT = CURRENT_TIME;
        this->verboseSummary<<CURRENT_TIME<<" "<<p->pCount<<" "<<p->timeInPrevState<<": Done"<<endl;
      }
      //cout<<"resetting current running process to "<<CURRENT_RUNNING_PROCESS<<endl;
    }
    if (i->oldState!="CREATED" && p->remainingExecTime!=0){
      this->verboseSummary<<CURRENT_TIME<<" "<<p->pCount<<" "<<p->timeInPrevState<<": "<< i->oldState<<" -> "<<i->newState;
    }
    
    

    if (count!=1){
    EventList::iterator iLookback = i;
    iLookback--;
    if (ioCount>0){
      ioTime+=CURRENT_TIME-iLookback->timestamp;
    }
    this->events.erase(iLookback);
    }

    if (i->oldState=="BLOCK"){
      ioCount--;
      p->totalIOTime+=p->timeInPrevState;
    }
    
    //going from created to ready
    if (i->newState =="READY"){
        if (p->remainingExecTime>0){
          sched->add_process(p);
        }
          CALL_SCHEDULER=true;

        if (i->oldState!="CREATED"&& i->oldState!="BLOCK")
          this->verboseSummary<<" cb="<<p->remainingCB<<" rem="<<p->remainingExecTime<<" prio="<<p->dynamic_priority<<endl;
        if (i->oldState == "BLOCK"){
          verboseSummary<<endl;
        }
    }

    //going from ready to run
    if (i->newState == "RUNNG"){
        int myRand;
        if (p->remainingCB==0){
          myRand = rand->myrandom(p->CB);
          p->remainingCB = myRand;
        } else {
          myRand = p->remainingCB;
        }
        if (p->remainingExecTime<myRand){
          myRand = p->remainingExecTime;
          p->remainingCB = myRand;
        }
        if (this->sched->quantum<myRand){
          Event runEvent(CURRENT_TIME+sched->quantum,p,"RUNNG","READY");
          this->insert_event(runEvent);            
        } else{
        Event runEvent(CURRENT_TIME+myRand,p,"RUNNG","BLOCK");
        this->insert_event(runEvent);
        }
        this->verboseSummary<<" cb="<<myRand<<" rem="<<p->remainingExecTime<<" prio="<<p->dynamic_priority<<endl;
    }


    if (i->newState == "BLOCK"){
        if (p->remainingExecTime==0){
          finalTime = CURRENT_TIME;
        } else {
          int myRand = rand->myrandom(p->IO);
          Event blockEvent(CURRENT_TIME+myRand,p,"BLOCK","READY");
          this->insert_event(blockEvent);
          this->verboseSummary<<" ib="<<myRand<<" rem="<<p->remainingExecTime<<endl;
          ioCount++;
        }
        CALL_SCHEDULER =true;
    }

    EventList::iterator iForward = i;
    iForward++;
   
    
    int nextTime = iForward->timestamp;
    //verboseSummary<<"nextTime: "<<nextTime<<", ioCount: "<<ioCount<<endl;
    
      if (CALL_SCHEDULER){
      
       if (nextTime==i->timestamp&&iForward != this->events.end()){
        //verboseSummary<<"encountered the same time stamp, advancing forward..."<<endl;
        continue;
       }
       CALL_SCHEDULER = false;
       //cout<<"current running process: "<<CURRENT_RUNNING_PROCESS<<endl;

       if (CURRENT_RUNNING_PROCESS==0){
           sched->get_next_process();
           CURRENT_RUNNING_PROCESS = sched->CURRENT_PROCESS;
           if (CURRENT_RUNNING_PROCESS == 0){
            if (iForward != this->events.end()){
            downTime+=nextTime-CURRENT_TIME;
            //verboseSummary<<"nextTime: "<<nextTime<<", CURRENT_TIME: "<<CURRENT_TIME<<", down Time: "<<downTime<<endl;
            }
            continue;
           }
           Event runEvent(i->timestamp,CURRENT_RUNNING_PROCESS,"READY","RUNNG");
           this->insert_event(runEvent);
       }
       //cout<<"process remaining cb: "<<p->remainingCB<<endl;
       //cout<<"run event old state: "<< runEvent.oldState<<", run event new state: "<<runEvent.newState<<", process pointer: "<<runEvent.process<<endl;
    }
    
}
    double avgTT=0;
    double avgWait=0;
    int counter = 0;
    if (verbose){
    cout<<verboseSummary.str()<<endl;
    }
    cout<<sched->schedulerAlgo<<endl;

    for (ProcessList::iterator p = this->processes.begin();p != this->processes.end();++p){
        //cout<<"wait time: "<<(*p)->TT<<endl;
        avgTT+=(*p)->TT-(*p)->AT;
        avgWait+=(*p)->totalWaitTime;
        //cout<<"count: "<<++counter<<", avgTT: "<<avgTT<<", avgWait: "<<avgWait<<endl;
       
        printf("%04d: %4d %4d %4d %4d %ld | %5d %5d %5d %5d\n",
          (*p)->pCount,
          (*p)->AT,
          (*p)->TC,
          (*p)->CB,
          (*p)->IO,
          (*p)->static_priority,
          (*p)->TT,
          (*p)->TT-(*p)->AT,
          (*p)->totalIOTime,
          (*p)->totalWaitTime
          );
       
        //cout<<"summary string being created: "<<summaryStream.str()<<endl;
    }
    double processSize = processes.size();
    avgTT/=processSize;
    avgWait/=processSize;



    printf("SUM: %d %.2lf %.2lf %.2lf %.2lf %.3lf\n",
    finalTime,
    100*(finalTime-downTime)/finalTime,
    100*ioTime/(finalTime),
    avgTT,
    avgWait,
    100*processSize/(double)finalTime);
    //cout<<"SUM: "<<<<" "<<<<" "<<<<" "<<<<" "<<;
}
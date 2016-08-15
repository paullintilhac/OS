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
double downTime = 0;
Process* CURRENT_RUNNING_PROCESS =0;
double ioCount=0;
double ioTime=0;
for (EventList::iterator i=this->events.begin();i != this->events.end();++i){
    Process* p = i->process;
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
      p->remainingExecTime -= p->timeInPrevState;
      p->remainingCB -= p->timeInPrevState;
      if (p->remainingExecTime == 0){
        p->TT = CURRENT_TIME;
        this->verboseSummary<<CURRENT_TIME<<" "<<p->pCount<<" "<<p->timeInPrevState<<": Done"<<endl;
      }
      //cout<<"resetting current running process to "<<CURRENT_RUNNING_PROCESS<<endl;
    }
    if (i->oldState!="CREATED" && p->remainingExecTime!=0)
      this->verboseSummary<<CURRENT_TIME<<" "<<p->pCount<<" "<<p->timeInPrevState<<": "<< i->oldState<<" -> "<<i->newState;
    if (i->oldState=="BLOCK"){
      ioCount--;
      p->totalIOTime+=p->timeInPrevState;
    }
     

    if (count!=1){
    EventList::iterator iLookback = i;
    iLookback--;
    this->events.erase(iLookback);
    }

  
    
    //going from created to ready
    if (i->newState =="READY"){
        //cout<<"adding process to run queue"<<endl;
        if (p->remainingExecTime>0){
          sched->add_process(p);
        }
          CALL_SCHEDULER=true;

        if (i->oldState!="CREATED")
          this->verboseSummary<<endl;
    }

    //going from ready to run
    if (i->newState == "RUNNG"){
        int myRand;
        //verboseSummary<<"myrand for CB: "<<myRand<<endl;
        if (p->remainingCB==0){
          //verboseSummary<<"saving myRandom as remaining CB: "<<myRand<<endl;
          myRand = rand->myrandom(p->CB);
          p->remainingCB = myRand;
        } else {
          //verboseSummary<<"using remaining time as CB: "<<p->remainingCB<<endl;
          myRand = p->remainingCB;
        }
        if (p->remainingExecTime<myRand){
          myRand = p->remainingExecTime;
          //cout<<"saving myrand as remaining exec time: "<<myRand<<endl;
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
          //verboseSummary<<"myRand for IB: "<<myRand<<endl;
          Event blockEvent(CURRENT_TIME+myRand,p,"BLOCK","READY");
          this->insert_event(blockEvent);
          this->verboseSummary<<" ib="<<myRand<<" rem="<<p->remainingExecTime<<endl;
        }
        ioCount++;
        CALL_SCHEDULER =true;
    }

    EventList::iterator iForward = i;
    iForward++;

    
    int nextTime = iForward->timestamp;
    
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
            //verboseSummary<<"process still null"<<endl;
            downTime+=nextTime-CURRENT_TIME;
            continue;
           }
           Event runEvent(i->timestamp,CURRENT_RUNNING_PROCESS,"READY","RUNNG");
           this->insert_event(runEvent);
       }
       //cout<<"process remaining cb: "<<p->remainingCB<<endl;
       //cout<<"run event old state: "<< runEvent.oldState<<", run event new state: "<<runEvent.newState<<", process pointer: "<<runEvent.process<<endl;
    }
    if (ioCount>0){
      ioTime+=nextTime-CURRENT_TIME;
    }
}
    double avgTT=0;
    double avgWait=0;
    int counter = 0;
    for (ProcessList::iterator p = this->processes.begin();p != this->processes.end();++p){
        avgTT+=(*p)->TT;
        avgWait+=(*p)->totalWaitTime;
        cout<<"count: "<<++counter<<", avgTT: "<<avgTT<<", avgWait: "<<avgWait<<endl;
        stringstream ss;
        ss << setw(4) << setfill('0') << (*p)->pCount;
        string s = ss.str();        
        //cout<<"creating process summary"<<endl;
        stringstream summaryStream;
        summaryStream<<s<<" "<<(*p)->AT<<" "<<(*p)->TC<<" "<<(*p)->CB<<" "<<(*p)->IO<<" "<<(*p)->static_priority<<" |  "
            <<(*p)->TT<<" "<<(*p)->TT-(*p)->AT<<" "<<(*p)->totalIOTime<<" "<<(*p)->totalWaitTime<<endl;
        //cout<<"summary string being created: "<<summaryStream.str()<<endl;
        this->processSummary<<summaryStream.str();
    }
    cout<<"processes size: "<<processes.size()<<endl;
    double processSize = processes.size();
    cout<<"avgTT: "<<avgTT<<endl;
    avgTT/=processSize;
    cout<<"avgTT: "<<avgTT<<endl;
    avgWait/=processSize;

if (verbose){
    cout<<this->verboseSummary.str()<<endl;
}
    cout<<this->sched->schedulerAlgo<<endl;
    cout<<this->processSummary.str();
    printf("SUM: %d %.2lf %.2lf %.2lf %.2lf %.3lf\n",
      finalTime,
      (finalTime-downTime)/finalTime,
      ioTime/(finalTime),
      avgTT,
      avgWait,
      100*processSize/(double)finalTime);
    //cout<<"SUM: "<<<<" "<<<<" "<<<<" "<<<<" "<<;
}
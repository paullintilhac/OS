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

   	cout<<"event list size: "<<sched->ioQueue.size()<<endl;

}


Sim::run_simulation(){
	for (IOEventList::iterator i = sched->ioQueue.begin();i!=sched->ioQueue.end();++i){
		
	}
}

Sim::insert_event(IOEvent e){

	IOEventList::iterator inserter = sched->ioQueue.begin();

 	while (++inserter != sched->ioQueue.end() && inserter->timestamp<= e.timestamp){
    //do nothing else
 	};

  
	sched->ioQueue.insert(inserter, e);
  
}
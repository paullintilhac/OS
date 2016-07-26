#ifndef SCHED_H
#define SCHED_H
#include <list>
#include <string>
#include "Util.h"
using namespace std;

typedef list<IOEvent> IOEventList;
typedef list<IOProcess*> IOProcessList;
class Sched{
public:
	IOEventList ioQueue;
	IOProcessList ioProcesses;
	IOProcessList readyQueue;
	Sched(string iFile)
	{
		ifstream infile (iFile.c_str());
		string str;

		while (getline(infile, str)) {
	    istringstream iss(str);

	    char first = str.at(str.find_first_not_of(" \t\r\n"));
	    if (first == '#'){
	    	continue;
	    }
	    int time, track;
	 	
	    if (!(iss >> time >>track)) {
	    	cout<<"iss is false in if statement"<<endl;
	    	break;
	    }
	    IOProcess* process = new IOProcess(time, track);
	    ioProcesses.push_back(process);

	    IOEvent event(time,process, "CREATED", "READY");
	    ioQueue.push_back(event);
	 	//cout<<"time: "<<time<<", track: "<<track<<endl; 	
	 	}
	};
	void add_process(IOProcess* p){
		readyQueue.push_back(p);
	}
	virtual IOProcess* get_next_process(){
	//do nothing
	}
};

class FIFO : public Sched {
public:
	FIFO(string iFile) : Sched(iFile){
	//cout<<"constructing fifo"<<endl;
	}
	IOProcess* get_next_process(){
		IOProcess* CURRENT_PROCESS;
		if (readyQueue.size()>0){
		CURRENT_PROCESS = readyQueue.front();
		readyQueue.pop_front();
		} else{
			CURRENT_PROCESS = NULL;
		}
		return CURRENT_PROCESS;
	}
};

class SCAN : public Sched {
public:
	SCAN(string iFile) : Sched(iFile){

	}
	IOProcess* get_next_process(){

	}
};

class FSCAN : public Sched {
public:
	FSCAN(string iFile) : Sched(iFile){

	}
	IOProcess* get_next_process(){

	}
};

class SSTF : public Sched {
public:
	SSTF(string iFile) : Sched(iFile){
		
	}
	
	IOProcess* get_next_process(){

	}
};

class CSCAN : public Sched {
public:
	CSCAN(string iFile) : Sched(iFile){

	}
	IOProcess* get_next_process(){

	}
};



#endif

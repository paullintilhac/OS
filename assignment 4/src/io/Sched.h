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
	int PREV_TRACK;
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
	 	PREV_TRACK=0;
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
		//cout<<"finished constructor of SSTF"<<endl;
	}
	
	IOProcess* get_next_process(){
		IOProcessList::iterator erasor = readyQueue.begin();
		IOProcess* CURRENT_PROCESS;
		if (readyQueue.size()>0){
			int minDistance = abs(readyQueue.front()->track-PREV_TRACK);
			int count = 0;

			for (IOProcessList::iterator i = readyQueue.begin();i!=readyQueue.end();++i){
				//cout<<"count: "<<count++<<endl;;
				int distance = abs((*i)->track-PREV_TRACK);
				if (distance<minDistance){
					minDistance = distance;
					erasor = i;
				}
			}
			CURRENT_PROCESS = *erasor;
			readyQueue.erase(erasor);
		}else{
			CURRENT_PROCESS= NULL;
		}
		return CURRENT_PROCESS;
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

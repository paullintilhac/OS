#ifndef SCHED_H
#define SCHED_H
#include <list>
#include <string>
#include "Util.h"
using namespace std;

typedef list<IOEvent> IOEventList;

class Sched{
public:
	IOEventList ioQueue;
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
	    IOEvent event(time,track);
	    ioQueue.push_back(event);
	 	cout<<"time: "<<time<<", track: "<<track<<endl; 	
	 	}
	};

	virtual int next_operation(){
	//do nothing
	}
};

class FIFO : public Sched {
public:
	FIFO(string iFile) : Sched(iFile){

	}
	int next_operation(){

	}
};

class SCAN : public Sched {
public:
	SCAN(string iFile) : Sched(iFile){

	}
	int next_operation(){

	}
};

class FSCAN : public Sched {
public:
	FSCAN(string iFile) : Sched(iFile){

	}
	int next_operation(){

	}
};

class SSTF : public Sched {
public:
	SSTF(string iFile) : Sched(iFile){
		
	}
	
	int next_operation(){

	}
};

class CSCAN : public Sched {
public:
	CSCAN(string iFile) : Sched(iFile){

	}
	int next_operation(){

	}
};



#endif

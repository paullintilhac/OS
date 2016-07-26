#ifndef UTIL_H
#define UTIL_H
using namespace std;

class IOProcess{
public:
	IOProcess(int startTime){
		id =startTime; 
		state="READY";
	}
	string state;
	int id; 
	int state_ts;
};

class IOEvent{
public:
	int timestamp, track;
	string oldState, newState;
	IOProcess* process;

	IOEvent(int arrivalT, int t) {
		track = t;
		timestamp = arrivalT;
		oldState = "CREATED";
		newState = "READY";
		process = new IOProcess(arrivalT);
	}
};




#endif
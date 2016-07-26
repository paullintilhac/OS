#ifndef UTIL_H
#define UTIL_H
using namespace std;

class IOProcess{
public:
	IOProcess(int startTime, int t){
		state_ts = startTime;
		startTS = startTime;
		track  = t;
	}
 	int startTS,doneTS;
	int track;
	int state_ts;
	int time_in_prev_state;
};

class IOEvent{
public:
	string oldState, newState;
	IOProcess* process;
	int timestamp;
	IOEvent(int time, IOProcess* p,string oldSt, string newSt) {

		timestamp = time;
		oldState = oldSt;
		newState = newSt;
		process = p;	
		process->state_ts = time;

	}
};




#endif
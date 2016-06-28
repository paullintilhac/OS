#ifndef EVENT_H
#define EVENT_H
#include "Process.h"
#include "State.h"

class Event{
public:
    Event(int timestamp,Process *process,State oldState, State newState);
	int timestamp;
	Process *process;
	State oldState;
	State newState;
};

#endif
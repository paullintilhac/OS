#ifndef EVENT_H
#define EVENT_H
#include "Process.h"

class Event{
public:
    Event(int timestamp,Process *process,int oldState, int newState);
	int timestamp;
	Process *process;
	int oldState;
	int newState;
};

#endif
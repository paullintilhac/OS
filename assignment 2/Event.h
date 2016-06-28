#ifndef EVENT_H
#define EVENT_H
#include "Process.h"
#include <string>
class Event{
public:
    Event(int timestamp,Process *process,std::string oldState, std::string newState);
	int timestamp;
	Process *process;
	std::string oldState;
	std::string newState;
};

#endif
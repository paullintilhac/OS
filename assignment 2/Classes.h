
class Process{
public:
    Process(int AT,int TC, int CB, int IO);
    ~Process();
    int AT;
    int TC;
    int CB;
    int IO;
};

class Event{
public:
    Event(int timestamp,Process *process,int oldState, int newState);
    ~Event();
	int timestamp;
	Process *process;
	int oldState;
	int newState;
};


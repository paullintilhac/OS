#include <string>
#include <list>
#include <sstream>
#include <fstream>
#include "Event.h"
#include "Process.h"
#include "Scheduler.h"
#include "Rand.h"
using namespace std;
typedef list<Event> EventList;
typedef list<Process*> ProcessList;
class DES{
public:
	DES(string originFile,Rand* rand,Scheduler* sched);
	insert_event(Event e);
	run_simulation(bool verbose);
	EventList events;
	int START_TIME;
	int CURRENT_TIME;
	bool CALL_SCHEDULER;
    ProcessList processes;
	Process *CURRENT_RUNNING_PROCESS;
	Rand* rand;
	Scheduler* sched;
	stringstream processSummary;
	stringstream verboseSummary;
	stringstream schedulerSummary;
};




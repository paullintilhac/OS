#include <string>
#include <list>
#include <sstream>
#include "Event.h"
#include "Process.h"
#include "Scheduler.h"
#include "Rand.h"
using namespace std;
typedef list<Event> EventList;

class DES{
public:
	DES(string originFile,Rand* rand,Scheduler* sched);
	insert_event(Event e);
	run_simulation();
	EventList events;
	int CURRENT_TIME;
	bool CALL_SCHEDULER;
	Process *CURRENT_RUNNING_PROCESS;
	Rand* rand;
	Scheduler* sched;
	stringstream processSummary;
	stringstream verboseSummary;
	stringstream schedulerSummary;
};




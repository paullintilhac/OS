#ifndef SCHEDULER_H
#define SCHEDULER_H
#include "Process.h"
#include <queue>
#include <string>
using namespace std;
class Scheduler{
public:

	Scheduler(string type);
	add_process(Process *p);
	get_next_process();
	string schedulerAlgo;
	Process *CURRENT_PROCESS;
	queue<Process*> readyQueue;

};

#endif
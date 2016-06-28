#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <typeinfo>
#include <list>
#include <algorithm>
#include <queue>
#include "Event.h"
#include "Process.h"
#include "Rand.h"
#include "Scheduler.h"
#include "DES.h"
using namespace std;
typedef list<Event> EventList;


int main (int argc, char* argv[]) {
string inputFileName = argv[1];
string randFileName = argv[2];
Rand rand(randFileName);
Scheduler sched("fcfs");

printf("running with inputfile %s and randfile %s\n",argv[1],argv[2]);

DES des(argv[1],&rand,&sched);
EventList::iterator b = des.events.end();
b--;
cout<<"CB at back of list in main: "<<b->process->CB<<endl;

des.run_simulation();
//read in randfile


return 0;
}
 
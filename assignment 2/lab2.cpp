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
typedef list<string> StringList;

int main (int argc, char* argv[]) {
string inputFileName = argv[1];
string randFileName = argv[2];
Rand rand(randFileName);
Scheduler sched("fcfs");

printf("running with inputfile %s and randfile %s\n",argv[1],argv[2]);

DES des(argv[1],&rand,&sched);

des.run_simulation();


return 0;
}
 
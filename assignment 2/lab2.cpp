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
#include "State.h"
#include "Rand.h"
#include "Scheduler.h"
using namespace std;


int main (int argc, char* argv[]) {
ifstream infile (argv[1]);
Rand rand(argv[2]);
Scheduler sched("fcfs");
printf("running with inputfile %s and randfile %s\n",argv[1],argv[2]);
int count =0;
typedef list<Event> EventList;
EventList events;

string str;

while (getline(infile, str)) {
    // output the line
    //cout << str << endl;
    count++;
    istringstream iss(str);

    int AT,TC,CB,IO;
    if (!(iss >> AT >> TC>>CB>>IO)) { 
    	cout<<"breaking out of while loop \n"; 
    	break;
    } 
	Process p(AT,TC,CB,IO);

	Event evt(AT,&p,CREATED,READY);
    events.push_back(evt);
   //iss>>a>>b>>c>>d;
    cout<<"AT: "<<AT<<", TC: "<<TC<<", CB: "<<CB<<", IO: "<<IO<<endl;

    // now we loop back and get the next line in 'str'
}

//read in randfile



//start DES

int CURRENT_TIME=0;
bool CALL_SCHEDULER=false;

for (EventList::iterator i = events.begin(); i != events.end(); ++i){

    //going from created to ready
    if ( i->newState ==READY){
        cout<<"adding process to run queue"<<endl;
        sched.add_process(i->process);
        CALL_SCHEDULER=true;
    }

    //going from ready to run
    if (i->newState == RUNNING){
        int myRand = rand.myrandom(i->process->CB);
        cout<<"random number used for blocking event: "<<myRand<<endl;
        //create event for either preemption or blocking
        CALL_SCHEDULER = true;
    }
    if (i->newState == IO){
        int myRand = rand.myrandom(i->process->IO);
        cout<<"random number used for CPU event: "<<myRand<<endl;
        //create an event for when the process becomes ready again
        CALL_SCHEDULER =true;
    }
    if (CALL_SCHEDULER){

    }

    cout<<"count: "<<(i->timestamp)<<endl;
}


printf("list length: %d\n",events.size());

cout<<count<<"\n";
cout<<"done with while loop\n";
infile.close();

return 0;
}
 
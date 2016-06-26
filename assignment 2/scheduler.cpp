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
using namespace std;


int main (int argc, char* argv[]) {
ifstream infile (argv[1]);

string str;
printf("running with file %s\n",argv[1]);
int count =0;
typedef list<Event> EventList;
EventList events;
queue<Process*> *readyQueue = new queue<Process*>;
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

//start DES

int CURRENT_TIME=0;
bool CALL_SCHEDULER=false;
for (EventList::iterator i = events.begin(); i != events.end(); ++i){

    //going from created to ready
    if (i->oldState == CREATED && i->newState ==READY){
        readyQueue->push(i->process);
    }

    //going from ready to run
    cout<<"count: "<<(i->timestamp)<<endl;
}


printf("list length: %d\n",events.size());

cout<<count<<"\n";
cout<<"done with while loop\n";
infile.close();

return 0;
}
 
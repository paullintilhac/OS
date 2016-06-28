#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <typeinfo>
#include <list>
#include <algorithm>
#include <queue>
using namespace std;
class Process{
public:
    Process(int CB);  
    int CB;
};

Process::Process(int c){
    CB = c;  
}

class Event{
public:
    Event(Process *process);
	Process *process;	
};

Event::Event(Process *ps){
    process = ps;
}

typedef list<Event> EventList;

class DES{
public:
	DES(string originFile);
	EventList events;
};



DES::DES(string originFile){
	ifstream infile (originFile.c_str());
	string str;
	while (getline(infile, str)) {
	    // output the line
	    //cout << str << endl;
	    istringstream iss(str);

	    int AT,TC,CB,IO;
	    if (!(iss >> AT >> TC>>CB>>IO)) { 
	    	cout<<"breaking out of while loop \n"; 
	    	break;
	    } 
		  Process p(CB);
		  Event evt(&p);
	    this->events.push_back(evt);
    
	}

  int cb =  this->events.back().process->CB;
EventList::iterator inserter2 = this->events.begin();
EventList::iterator inserter3 = this->events.end();

//inserter3--;

//cout<<"CB after while loop using List<>::end(): " <<inserter3->process->CB<<endl;
 //cout<<"CB after while loop using LIST<>::back "<<cb<<endl;

    infile.close();

}


int main (int argc, char* argv[]) {
string inputFileName = argv[1];


DES des(argv[1]);
EventList::iterator b = des.events.end();
b--;
cout<<"CB at back of list in main: "<<b->process->CB<<endl;

return 0;
}
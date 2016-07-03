#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <typeinfo>
#include <list>
#include <algorithm>
#include <queue>
#include <regex>
#include "Event.h"
#include "Process.h"
#include "Rand.h"
#include "Scheduler.h"
#include "DES.h"
using namespace std;
typedef list<Event> EventList;
typedef list<string> StringList;

int main (int argc, char* argv[]) {
string schedName;
bool verbose;
int quantum=100000000;
int numOptions = argc-3;

regex reg ("-s(R|P)[0-9]+");

//cout<<"regex match: "<<(regex_match("-sP",reg))<<endl;


for (int i =0;i<numOptions;i++){
	bool validOption =false;
	string compareString= string("-v");
	string optionString  = string(argv[i+1]);

	if (compareString==optionString || optionString == string("-sF") || optionString == string("-sL") ||optionString == string("-sS")||regex_match(argv[i+1],reg)){
		validOption = true;
	}
	if (!validOption){
		cout<<"INVALID OPTION!!"<<endl;
		exit(1);
	}
	if (compareString==optionString)
		verbose = true;
	if (optionString==string("-sF"))
		schedName = "FCFS";
	if (optionString==string("-sL"))
		schedName = "LCFS";
	if (optionString==string("-sS"))
		schedName = "SJF";
	if (regex_match(argv[i+1],reg)){
		string quantumString = optionString.substr(3,optionString.length());
		quantum = atoi(quantumString.c_str());
		char schedType =optionString[2];
		if (schedType=='R')
			schedName = "RR";
		if (schedType=='P')
			schedName = "PRIO";
	}
}

//cout<<"sched name: *"<<schedName<<"*"<<endl;

string inputFileName = argv[numOptions+1];
string randFileName = argv[numOptions+2];
Rand rand(randFileName);
Scheduler* sched;

if (schedName == "FCFS"){
sched = new FCFS();
}
if (schedName == "LCFS"){
sched = new LCFS();
}
if (schedName=="SJF"){
sched = new SJF(); 
}
if (schedName=="RR"){
sched = new RR(quantum); 
}
if (schedName=="PRIO"){
sched = new PRIO(quantum); //placeholder for now
}

DES des(argv[numOptions+1],&rand,sched);
des.run_simulation(verbose);

return 0;
}
 
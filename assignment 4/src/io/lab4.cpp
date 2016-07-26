#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <ctype.h>
#include  <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "Sim.h"
using namespace std;
int main(int argc,char* argv[]){

	string algoString  = string(argv[1]);
	string startChar = algoString.substr(0,2);
	if (startChar!="-s"){
		cout<<"invalid option: -s<algo name>"<<endl;
	}
	string rest = algoString.substr(2,algoString.length()-2);
	string inputFileName = string(argv[2]);

 	//cout<<"algo name: "<<algoName<<", physical: "<<physical<<", O: "<<O<<", P: "<<P<<", F: "<<F<<", S: "<<S<<endl;
	Sim sim(inputFileName, rest);
	sim.run_simulation();
}
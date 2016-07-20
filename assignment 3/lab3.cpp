#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "Tables.h"
#include "Sim.h"
#include "Pager.h"
using namespace std;
int main(int argc, char* argv[]){
string algoName;
bool O;
bool P;
bool F;
bool S;
int quantum=100000000;
int numOptions = argc-3;
int nFrames = 4;



	string inputFileName = argv[numOptions+1];
	string randFileName = argv[numOptions+2];
	cout<<"nFrames: "<<nFrames<<endl;
	Sim sim(inputFileName,nFrames);
	ifstream randFile (randFileName.c_str());
}
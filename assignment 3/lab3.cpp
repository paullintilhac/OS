#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "Tables.h"
#include "Sim.h"
#include "Pager.h"
#include "Rand.h"
using namespace std;
int main(int argc, char* argv[]){
string algoName="LRU";
bool O;
bool P;
bool F;
bool S;
int quantum=100000000;
int numOptions = argc-3;
int nFrames = 4;
bool physical=true;
for (int i=0;i<numOptions;++i){
	string optionString  = string(argv[i+1]);
	string startChar = optionString.substr(0,2);
	string rest = optionString.substr(2,optionString.length()-2);
	
		for (int j=0;j<rest.length();j++){

			if (startChar == "-o"){
				if (rest.at(j)=='O'){
					O=true;}
				if (rest.at(j)=='F'){
					F=true;}
				if (rest.at(j)=='P'){
					P=true;}
				if (rest.at(j)=='S'){
					S=true;}
			}
			if (startChar == "-a"){
				if (rest.at(j)=='N'){
					algoName="NRU";
					physical = false;}
				if (rest.at(j)=='l'){
					algoName = "LRU";
					physical = true;}
				if (rest.at(j)=='r'){
					algoName = "Random";
					physical=true;}
				if (rest.at(j)=='f'){
					algoName = "FIFO";
					physical = true;}
				if (rest.at(j)=='s'){
					algoName = "SC";
					physical = true;}
				if (rest.at(j)=='c'){
					algoName = "Clock";
					physical = true;}
				if (rest.at(j)=='X'){
					algoName = "Clock";
					physical= false;}
			}
			if (startChar == "-f"){
				nFrames = stoi(rest);
			}
	}
}
 	//cout<<"algo name: "<<algoName<<", physical: "<<physical<<", O: "<<O<<", P: "<<P<<", F: "<<F<<", S: "<<S<<endl;

	string inputFileName = argv[numOptions+1];
	string randFileName = argv[numOptions+2];
	ifstream randFile (randFileName.c_str());
	Rand* rand = new Rand(randFileName);
	Sim sim(inputFileName,nFrames, algoName, physical,O,P,F,S,rand);
}
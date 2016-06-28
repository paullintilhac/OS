#ifndef PROCESS_H
#define PROCESS_H
#include "State.h"
class Process{
public:
    Process(int AT,int TC, int CB, int IO);
    int AT;
    int TC;
    int CB;
    int IO;
    State state;
};
#endif
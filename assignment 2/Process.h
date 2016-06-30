#ifndef PROCESS_H
#define PROCESS_H
#include <string>
class Process{
public:
    Process(int AT,int TC, int CB, int IO, int prio, int pCount);
    int AT;
    int TC;
    int CB;
    int IO;
    int TT;
    std::string state;
    int timeInPrevState;
    int state_ts;
    int remainingExecTime;
    int remainingCB;
    int totalIOTime;
    int totalWaitTime;
    int static_priority;
    int dynamic_priority;
    int pCount;
};
#endif
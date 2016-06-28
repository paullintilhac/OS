#ifndef PROCESS_H
#define PROCESS_H
class Process{
public:
    Process(int AT,int TC, int CB, int IO);
    int AT;
    int TC;
    int CB;
    int IO;
    int state;
    int timeInPrevState;
    int state_ts;
    int remainingExecTime;
};
#endif
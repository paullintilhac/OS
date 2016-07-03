#include "Process.h"
Process::Process(int a,int t,int c, int i,int prio,int count){
    AT = a;
    TC = t;
    CB = c;
    IO=i;
    state ="CREATED";
    timeInPrevState = 0;
    remainingExecTime = TC;
    remainingCB = 0;
    totalIOTime = 0;
    totalWaitTime =0;
    static_priority = prio;
    dynamic_priority =prio-1;
    active=true;
    pCount =count;
}

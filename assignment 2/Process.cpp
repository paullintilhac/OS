#include "State.h"
#include "Process.h"
Process::Process(int a,int t,int c, int i){
    AT = a;
    TC = t;
    CB = c;
    IO=i;
    state =READY;
}

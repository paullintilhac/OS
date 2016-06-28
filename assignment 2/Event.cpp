#include "Event.h"
Event::Event(int ts,Process *ps,int os,int ns){
    timestamp = ts;
    process = ps;
    oldState = os;
    newState = ns;
}


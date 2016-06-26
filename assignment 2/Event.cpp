#include "Event.h"
Event::Event(int ts,Process *ps,State os,State ns){
    timestamp = ts;
    process = ps;
    oldState = os;
    newState = ns;
}


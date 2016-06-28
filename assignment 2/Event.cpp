#include "Event.h"
#include <string>
Event::Event(int ts,Process *ps,std::string os,std::string ns){
    timestamp = ts;
    process = ps;
    oldState = os;
    newState = ns;
}


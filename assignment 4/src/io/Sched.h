#ifndef SCHED_H
#define SCHED_H
#include <list>
#include <string>
#include "Util.h"
using namespace std;

typedef list<IOEvent> IOEventList;
typedef list<IOProcess*> IOProcessList;
class Sched{
public:
	IOEventList ioQueue;
	IOProcessList ioProcesses;
	IOProcessList readyQueue;
	IOProcessList activeQueue;

	int PREV_TRACK;
	int end_track;
	int prev_end_track;
	int MAX_TRACK;
	int direction;
	Sched(string iFile)
	{ 
		direction = 1;
		ifstream infile (iFile.c_str());
		string str;
		int id = 0;
		while (getline(infile, str)) {
	    istringstream iss(str);

	    char first = str.at(str.find_first_not_of(" \t\r\n"));
	    if (first == '#'){
	    	continue;
	    }
	    int time, track;
	 	
	    if (!(iss >> time >>track)) {
	    	cout<<"iss is false in if statement"<<endl;
	    	break;
	    }
	    if (track>MAX_TRACK){
	    	MAX_TRACK = track;
	    }
	    IOProcess* process = new IOProcess(time, track,id);
	    ioProcesses.push_back(process);

	    IOEvent event(time,process, "CREATED", "READY");
	    ioQueue.push_back(event);
	 	//cout<<"time: "<<time<<", track: "<<track<<endl; 	
	 	++id;
	 	}
	 	//cout<<"processes size: "<<ioProcesses.size()<<", ioQueue size: "<<ioQueue.size()<<endl;
	 	end_track = 0;
	 	prev_end_track = 0;
	 	PREV_TRACK=0;
	 	
	};
	virtual add_process(IOProcess* p){
		if (direction==1 && p->track>PREV_TRACK && p->track>end_track){
			//cout<<"resetting end process in forward direction: process "<<p->id<<", new end track: "<<p->track<<endl;
			end_track = p->track;
		}
		if (direction==0 && p->track<PREV_TRACK && p->track<end_track){
			end_track=p->track;
			//cout<<"resetting end process in backward direction: process "<<p->id<<", new end track: "<<p->track<<endl;
		}
		readyQueue.push_back(p);
	}
	virtual IOProcess* get_next_process(){
	//do nothing
	}
};

class FIFO : public Sched {
public:
	FIFO(string iFile) : Sched(iFile){
	//cout<<"constructing fifo"<<endl;
	}
	IOProcess* get_next_process(){
		IOProcess* CURRENT_PROCESS;
		if (readyQueue.size()>0){
		CURRENT_PROCESS = readyQueue.front();
		readyQueue.pop_front();
		} else{
			CURRENT_PROCESS = NULL;
		}
		return CURRENT_PROCESS;
	}
};

class SCAN : public Sched {
public:
	SCAN(string iFile) : Sched(iFile){
	}
	IOProcess* get_next_process(){
		IOProcessList::iterator erasor = readyQueue.begin();
		IOProcess* CURRENT_PROCESS;
		//cout<<"PREV TRACK: "<<PREV_TRACK<<", end track: "<<end_track<<", direction: "<<direction<<endl;
		int max_track = 0;
		int min_track = readyQueue.front()->track;
		if (PREV_TRACK == end_track){
			//cout<<"resetting direction"<<endl;
			direction = 1-direction;
		}
		
		if (readyQueue.size()>0){
			int minDistance = MAX_TRACK;
			int count = 0;

			for (IOProcessList::iterator i = readyQueue.begin();i!=readyQueue.end();++i){
				//cout<<"count: "<<count++<<endl;;
				if ((*i)->track>max_track){
					max_track = (*i)->track;
				}
				if ((*i)->track<min_track){
					min_track = (*i)->track;
				}
				int distance = (*i)->track-PREV_TRACK;
				if (abs(distance)<minDistance){
					if (direction ==1 && distance>=0){
					erasor = i;
					minDistance = abs(distance);
					}

					if (direction ==0 && distance<=0){
					minDistance = abs(distance);
					erasor = i;
					}
				}
			}
		if (PREV_TRACK == end_track){
			if (direction ==1){
			end_track = max_track;
			}
			if (direction == 0){
			end_track = min_track;
			}
		}	

			CURRENT_PROCESS = *erasor;
			
			readyQueue.erase(erasor);
		}else{
			CURRENT_PROCESS= NULL;
		}
		return CURRENT_PROCESS;	
	}
};

class FSCAN : public Sched {
public:
	FSCAN(string iFile) : Sched(iFile){
	}

	IOProcess* get_next_process(){
		IOProcess* CURRENT_PROCESS;
		//cout<<"PREV_TRACK: "<<PREV_TRACK<<", end track: "<<end_track<<", prev_end_track: "<<prev_end_track<<", active queue size: "<<activeQueue.size()<<"ready queue size: "<<readyQueue.size()<<endl;

		if (PREV_TRACK == prev_end_track){
			direction = 0;
			//cout<<"resetting direction to 0"<<endl;
		}

		if (activeQueue.size()==0){
			if (PREV_TRACK<=end_track){
			direction = 1;
			}else{
			direction = 0;
			}
			prev_end_track = end_track;

			//cout<<"swapping queues -- resetting direction to 1"<<endl;
			IOProcessList tempQueue = readyQueue;
			readyQueue = activeQueue;
			activeQueue = tempQueue;
			end_track = 0;
		}
		IOProcessList::iterator erasor = activeQueue.begin();
		int max_track = 0;
		int min_track = readyQueue.front()->track;
		if (activeQueue.size()>0){
			int minDistance = MAX_TRACK;
			int count = 0;

			for (IOProcessList::iterator i = activeQueue.begin();i!=activeQueue.end();++i){
				//cout<<"count: "<<count++<<endl;;
				if ((*i)->track>max_track){
					max_track = (*i)->track;
				}
				if ((*i)->track<min_track){
					min_track = (*i)->track;
				}
				int distance = (*i)->track-PREV_TRACK;

				if (abs(distance)<minDistance){
					if (direction ==1 && distance>=0){
					erasor = i;
					minDistance = abs(distance);
					}

					if (direction ==0 && distance<=0){
					minDistance = abs(distance);
					erasor = i;
					}
					//cout<<"new min distance: "<<minDistance<<", current track: "<<(*i)->track<<", PREV_TRACK: "<<PREV_TRACK<<" distance: "<<distance<<endl;

				}
			}
			
			CURRENT_PROCESS = *erasor;
			
			activeQueue.erase(erasor);
		} else {
			CURRENT_PROCESS= NULL;
		}
		//printf("next process: %p\n",CURRENT_PROCESS);
		return CURRENT_PROCESS;	
	}
	add_process(IOProcess* p){
		//note end track gets incremented in the opposite direction from SCAN and CSCAN, since it is really the inactive queue limit
		if ( p->track>end_track){
			//cout<<"resetting end process in forward direction: process "<<p->id<<", new end track: "<<p->track<<endl;
			end_track = p->track;
		}
	
		readyQueue.push_back(p);
	}
};

class SSTF : public Sched {
public:
	SSTF(string iFile) : Sched(iFile){
		//cout<<"finished constructor of SSTF"<<endl;
	}
	
	IOProcess* get_next_process(){
		IOProcessList::iterator erasor = readyQueue.begin();
		IOProcess* CURRENT_PROCESS;
		if (readyQueue.size()>0){
			int minDistance = MAX_TRACK;
			int count = 0;

			for (IOProcessList::iterator i = readyQueue.begin();i!=readyQueue.end();++i){
				//cout<<"count: "<<count++<<endl;;
				int distance = abs((*i)->track-PREV_TRACK);
				if (distance<minDistance){
					minDistance = distance;
					erasor = i;
				}
				//cout<<"current track: "<<(*i)->track<<", prev track: "<<PREV_TRACK<<", distance: "<<distance<<endl;
			}
			CURRENT_PROCESS = *erasor;
			readyQueue.erase(erasor);
		}else{
			CURRENT_PROCESS= NULL;
		}
		return CURRENT_PROCESS;
	}
};

class CSCAN : public Sched {
public:
	CSCAN(string iFile) : Sched(iFile){

	}
	IOProcess* get_next_process(){
	IOProcessList::iterator erasor = readyQueue.begin();
		IOProcess* CURRENT_PROCESS;


			
		if (readyQueue.size()>0){
			
			int minDistance = MAX_TRACK;
			int count = 0;
			int new_end_track=0;
			for (IOProcessList::iterator i = readyQueue.begin();i!=readyQueue.end();++i){
				//printf("ready queue pointer: %p\n",(*i));
				if ((*i)->track>new_end_track){
					new_end_track = (*i)->track;
				}
				int distance;
				if (PREV_TRACK==end_track){
					//cout<<"restting distance formula"<<endl;
					distance = (*i)->track;
				}
				else{
					//cout<<"regular distance calculation"<<endl;
					distance = (*i)->track-PREV_TRACK;
				}
				
				//cout<<"distance: "<<distance<<endl;
				if (distance<minDistance && distance>=0){	
					erasor = i;
					minDistance = distance;
					//cout<<"new min distance: "<<minDistance<<endl;
				}
			}
			if (PREV_TRACK == end_track){

					//cout<<"resetting end track"<<endl;
					end_track = new_end_track;
					//cout<<"resetting direction to "<<direction<<endl;
			}
			
			CURRENT_PROCESS = *erasor;
			
			readyQueue.erase(erasor);
		}else{
			CURRENT_PROCESS= NULL;
		}
		return CURRENT_PROCESS;	
	}
};



#endif

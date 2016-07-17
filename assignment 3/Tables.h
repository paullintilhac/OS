#ifndef TABLES_H
#define TABLES_H
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <memory>

using namespace std;
class Page{
public:
	Page(){
		PRESENT = 0;
		MODIFIED = 0;
		REFERENCED = 0;
		PAGEDOUT = 0;
		referenceCount =0;
		frame = 0;
	}
	unsigned int PRESENT:1;
	unsigned int MODIFIED:1;
	unsigned int REFERENCED:1;
	unsigned int PAGEDOUT:1;
	unsigned int frame:10;
	int referenceCount;
};

class Frame{
public:
	Frame(){
		referenceCount = 0;
		locked = false;
		page = new Page();
	}
	Page *page;
	int referenceCount;
	bool locked;
	//also linkage, not sure what that is
};


#endif
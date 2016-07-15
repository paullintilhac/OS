#include <stdio>
#include <string>
#include <sstream>
#include <fstream>

class PTE{
public:
	bool PRESENT;
	bool MODIFIED;
	bool REFERENCED;
	bool PAGEDOUT;
}
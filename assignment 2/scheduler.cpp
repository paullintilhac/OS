#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;
ifstream infile("");
int main (int argc, char* argv[]) {


string line;
printf("hello1?");
while (getline(infile, line))
{
	printf("hello?");
    istringstream iss(line);
    int a, b;
    cout<<a <<b << iss;
    if (!(iss >> a >> b)) { break; } // error

    printf( "b: %d, a: %d\n" ,b,a); 
}
}

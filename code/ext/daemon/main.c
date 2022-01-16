#include <iostream>      //printf()

#include "CLdr.h"

using namespace std;

int main(int argc, char **argv)
{
	CLdr ldr;
	
	int lux = ldr.getLuxState();

	cout << "State: " << lux << endl;
	
    return 0; 
}
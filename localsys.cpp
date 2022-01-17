// #include "CLoraComm.h"
#include "CLocalSystem.h"
#include "debug.h"

#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
	CLocalSystem localsys;

	// CLoraComm lora(433, 0xcc, 0xbb);

	// lora.init(2);
	// cout << "lora.init() done" << endl;

	// lora.run();
	localsys.run();

	cout << endl << "Exit success!" << endl;
	return 0;
}
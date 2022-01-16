#include "CLoraComm.h"

#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
	CLoraComm lora(433, 0xcc, 0xbb);
	
	cout << "Exit success" << endl;

	return 0;
}
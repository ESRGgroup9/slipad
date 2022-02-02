#include "CGateway.h"
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
	CGateway gateway;

	gateway.run();
	
	cout << endl << "Gateway: Exited main()" << endl;
	return 0;
}
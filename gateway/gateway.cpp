#include "CGateway.h"
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
	if(argc < 3)
	{
		cout << "Usage: " << argv[0] << " <host> <portNum>" << endl;
		return 1;
	}

	CGateway gateway(argv[1], atoi(argv[2]));

	gateway.run();
	
	cout << endl << "Gateway: Exited main()" << endl;
	return 0;
}
#include "CGateway.h"
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
	stringstream debug_msg;

	if(argc < 3)
	{
		debug_msg << "Usage: " << argv[0] << " <host> <portNum>" << endl;
		DEBUG_MSG(debug_msg.srt().c_str());
		return 1;
	}

	CGateway gateway(argv[1], atoi(argv[2]));

	gateway.run();
	
	cout << endl << "Gateway: Exited main()" << endl;
	return 0;
}
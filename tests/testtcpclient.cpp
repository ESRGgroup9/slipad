#include "CTCPclient.h"
#include <string>
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
	if(argc < 3)
	{
		cout << "Usage: " << argv[0] << " <hostName> <port>" << endl;
		return 1;
	}

	string host = argv[1];
	int port = atoi(argv[2]);

	CTCPclient c1(host, port);
	c1.connect();

	return 0;
}
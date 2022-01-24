#include "CTCPclient.h"
#include <string>
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
	if(argc < 4)
	{
		cout << "Usage: " << argv[0] << " <hostName> <port> <msg1>...<msgN>" << endl;
		return 1;
	}

	string host = argv[1];
	int port = atoi(argv[2]);

	CTCPclient client(host, port);
	client.connect();
 	
 	for(int i = 3; i < argc; i++)
 	{
 		cout << "Sending [" << argv[i] << "]..." << endl;
 		client.send(argv[i]);
 	}

	return 0;
}
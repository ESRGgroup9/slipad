#include "CTCPserver.h"
#include <string>
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
	if(argc < 2)
	{
		cout << "Usage: " << argv[0] << " <port>" << endl;
		return 1;
	}

	int port = atoi(argv[1]);
	CTCPserver server(port);
	int sd = 0;

	while(1)
	{
		sd = server.accept();

		if(sd != -1)
		{
			cout << "New client on sockfd[" << sd << "]" << endl;
		}
	}

	return 0;
}
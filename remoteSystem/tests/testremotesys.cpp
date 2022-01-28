#include "CTCPclient.h"
#include <string>
#include <iostream>
using namespace std;

const static char *typeStr[] =
{
	"GATEWAY",
	"WEBSITE",
	"APPLICATION",
};

int main(int argc, char *argv[])
{
	if(argc < 4)
	{
		cout << "Usage: " << argv[0] << " <hostName> <port> <clientType>" << endl;
		return 1;
	}

	string host = argv[1];
	int port = atoi(argv[2]);

	CTCPclient client(host, port);
	client.connect();
 	
 	// -------------- run ----------------
 	cout << "Insert command after '>'" << endl;
 	cout << "Type 'q' to quit" << endl << endl;
 	string cmd;
 
 	// send client type
 	cmd = string("TYPE;") + argv[3];
 	client.send(cmd);
 	cout << ">" << cmd;

 	// print client type - verbose
 	int type = atoi(argv[3]);
 	if((type >= 0) && (type < 3))
 		cout << "   # Connecting as "<< typeStr[type];
 	
 	cout << endl;

 	while(1)
 	{
 		putchar('>');
	    getline(cin, cmd);

	    if(cmd.compare("q") == 0)
	    {
	    	break;
	    }
	    else
 			cout << "OK(" << client.send(cmd) << ")" << endl;
    }

    cout << "Bye" << endl;

	return 0;
}
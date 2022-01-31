#include "CTCPserver.h"
#include "utils.h"
#include "debug.h"

#include <cstring> // memset
#include <unistd.h> // gethostname

using namespace std;

const int CTCPserver::maxNumClients = 5;
int CTCPserver::numClients = 0;
  
CTCPserver::CTCPserver(int port)
{ 
	this->port = port;
	// create tcp socket using IPv4 and connection-based byte stream
	this->listenSd = socket(PF_INET, SOCK_STREAM, 0);
	
	if (listenSd < 0)
		// socket file descriptor not created
		panic("socket");

	// bind port/addr to socket 
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	// Translate an unsigned short integer into network byte order
	// x86 is litle-endian / network-order is big-endian
	addr.sin_port = htons(port);;
	// any interface
	addr.sin_addr.s_addr = INADDR_ANY;
	
	if(bind(listenSd, (struct sockaddr*)&addr, sizeof(addr)) != 0)
		panic("bind");

	// make listener with a maximum number of client slots
	if(listen(listenSd, maxNumClients) != 0)
		panic("listen")

	// get server host name
	char hostName[32];
	gethostname(hostName, sizeof(hostName));
	DEBUG_MSG("[CTCPserver] Starting server on " << hostName << ":" << port);
	DEBUG_MSG("[CTCPserver] Listening for incoming connections on sockfd: " << listenSd);
}

CTCPserver::~CTCPserver()
{

}

int CTCPserver::accept()
{
	socklen_t n = sizeof(addr);
	// // accept connections
	int sd = ::accept(listenSd, (struct sockaddr*)&addr, &n);

	if((sd != -1) && (numClients < maxNumClients))
	{
		// new client connected
		numClients++;
		DEBUG_MSG("[CTCPserver::accept] Client on sockfd["<< sd << "] connected (slot "<< numClients << "/" << maxNumClients << ")");
	}
	else
	{
		DEBUG_MSG("[CTCPserver::accept] Cannot accept new connection");
		// return -1 in case accept() returned a non zero value but the maximum
		// number of clients was reached
		sd = -1;
	}

	return sd;
}

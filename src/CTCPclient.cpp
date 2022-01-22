#include "CTCPclient.h"
#include "utils.h"
#include "debug.h"

#include <cstring>
#include <iostream>

#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

CTCPclient::CTCPclient(std::string host, int port)
{
	struct hostent* hostent;
	struct sockaddr_in addr;

	// Get server's IP and standard service connection
	hostent = gethostbyname(host.c_str());
	this->host = hostent->h_name;
	this->port = htons(port);

	// Create socket
	this->sockfd = socket(PF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		panic("socket");
	
	/* create & zero struct */
	memset(&addr, 0, sizeof(addr));
	/* select internet protocol */
	addr.sin_family = AF_INET;
	/* set the port # */
	addr.sin_port = this->port;
	/* set the addr */
	addr.sin_addr.s_addr = *(long*)(hostent->h_addr_list[0]);

	DEBUG_MSG("Connecting to server on " << hostent->h_name << ":" << this->port);
	// syslog(LOG_INFO, "Connecting to server on %s:%d\n", hostent->h_name, this->port);
	
	// get host name
	// gethostname(client_name, sizeof(client_name));
}

CTCPclient::~CTCPclient()
{

}

string CTCPclient::getHost(void) const
{
	return host;
}

int CTCPclient::getPort(void) const
{
	return port;
}

int CTCPclient::recvFunc(std::string &msg)
{
	return 0;
}

int CTCPclient::sendFunc(std::string msg)
{
	return 0;
}
#include "CTCPclient.h"
#include "utils.h"
#include "debug.h"

#include <cstring>
#include <iostream>

#include <netdb.h>
#include <sys/socket.h> 
#include <sys/types.h>
#include <unistd.h>

using namespace std;

CTCPclient::CTCPclient(std::string host, int port)
{
	struct hostent* hostent;

	// Get server's IP and standard service connection
	hostent = gethostbyname(host.c_str());
	this->host = hostent->h_name;
	this->port = port;

	// Create socket
	this->sockfd = socket(PF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		panic("socket");
	
	/* create & zero struct */
	memset(&addr, 0, sizeof(addr));
	/* select internet protocol */
	addr.sin_family = AF_INET;
	/* set the port # */
	addr.sin_port = htons(port);
	/* set the addr */
	addr.sin_addr.s_addr = *(long*)(hostent->h_addr_list[0]);

	// syslog(LOG_INFO, "Connecting to server on %s:%d\n", hostent->h_name, this->port);
	
	// get host name
	char str[32];
	gethostname(str, sizeof(str));
	DEBUG_MSG("[CTCPclient] '" << str << "' set to connect to " << host << ":" << port);

	// set status
	this->status = ConnStatus::ONLINE;
}

CTCPclient::~CTCPclient()
{
	// close the tcp connection
	shutdown(sockfd,SHUT_RD);
	shutdown(sockfd,SHUT_WR);
	shutdown(sockfd,SHUT_RDWR);	
	close(sockfd);
}

int CTCPclient::connect()
{
	int ret = 0;
	int err = 0;

	DEBUG_MSG("[CTCPclient::connect] Connecting to " << host << ":" << port << " ...");
	// connect to server
	do
	{
		ret = ::connect(sockfd, (struct sockaddr*)&addr, sizeof(addr));
		err = errno;
	}
	while((ret == -1) && (err == EAGAIN));

	if(ret == 0)
	{
		DEBUG_MSG("[CTCPclient::connect] Connection successful");	
	}
	else
	{
		ERROR_MSG("[CTCPclient::connect] Exit with return error[" << ret << "] errno[" << err << "]");
	}

	return ret;
}

int CTCPclient::recvFunc(std::string &msg)
{
	int ret = 0;
	char buffer[256];

	// recv message from server
	ret = ::recv(sockfd, buffer, sizeof(buffer), 0);
	if(ret == -1)
	{
		ERROR_MSG("[CTCPclient::recvFunc] return -1: " << string(strerror(errno)));
	}
	else if(ret == 0)
	{
		DEBUG_MSG("[CTCPclient::recvFunc] return 0: Stream socket peer has performed an orderly shutdown");
	}
	// else, return the number of bytes read
	else if(ret > 0)
	{
		// place null character at end of string
		buffer[ret] = '\0';
		// copy received message to msg
		msg = string(buffer);
		DEBUG_MSG("[CTCPclient::recvFunc] return " << ret << ": Received [" << msg << "]");
	}

	return ret;
}

int CTCPclient::sendFunc(std::string msg)
{
	int err = 0;

	// send message to server
	err = ::send(sockfd, msg.c_str(), msg.size(), 0);
	if(err == -1)
	{
		err = errno;
		ERROR_MSG("[CTCPclient::sendFunc] return -1: " << string(strerror(errno)));
	}
	// else, return the number of bytes sent
	return err;
}

string CTCPclient::getHost(void) const
{
	return host;
}

int CTCPclient::getPort(void) const
{
	return port;
}
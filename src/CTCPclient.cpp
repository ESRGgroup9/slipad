#include "CTCPclient.h"
#include "utils.h"
#include "debug.h"

#include <unistd.h> // gethostname
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
	
	// use sockfd in tcp communications
	tcp.setSockfd(sockfd);

	/* create & zero struct */
	memset(&addr, 0, sizeof(addr));
	/* select internet protocol */
	addr.sin_family = AF_INET;
	/* set the port # */
	addr.sin_port = htons(port);
	/* set the addr */
	addr.sin_addr.s_addr = *(long*)(hostent->h_addr_list[0]);

	// get host name
	char str[32];
	gethostname(str, sizeof(str));
	DEBUG_MSG("[CTCPclient] '" << str << "' set to connect to " << host << ":" << port);
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

string CTCPclient::getHost(void) const
{
	return host;
}

int CTCPclient::getPort(void) const
{
	return port;
}
#ifndef __CTCPSERVER_H__
#define __CTCPSERVER_H__

#include <arpa/inet.h> // sockaddr_in

class CTCPserver
{
public:
	CTCPserver(int port);
	~CTCPserver();

	int accept();

private:
	static const int maxNumClients;
	
	int listenSd;
	int port;
	struct sockaddr_in addr;
};

#endif // !__CTCPSERVER_H__

#ifndef __CTCPSERVER_H__
#define __CTCPSERVER_H__

// #include <arpa/inet.h> 
#include <netdb.h> // sockaddr_in
#include <vector>

class CTCPserver
{
public:
	CTCPserver(int port);
	~CTCPserver();

	int accept();

// private:
// 	static void *recvFcn(void *arg);
//     static void *sendFcn(void *arg);

private:
	static const int maxNumClients;
	static int numClients;
	// std::vector<int> clientsSd;

	int listenSd;
	int port;
	struct sockaddr_in addr;
};

#endif // !__CTCPSERVER_H__

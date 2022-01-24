#ifndef __CTCPCLIENT_H__
#define __CTCPCLIENT_H__

#include "CCommunication.h"
#include <string>
#include <netdb.h> // sockaddr_in

class CTCPclient : public CCommunication
{
public:
	CTCPclient(std::string host, int port);
	~CTCPclient();

	int connect();

	std::string getHost(void) const;
	int getPort(void) const;

protected:
	virtual int recvFunc(std::string &msg);
	virtual int sendFunc(std::string msg);

private:
	int sockfd;
	int port;
	std::string host;
	struct sockaddr_in addr;
};
// End CTCPclient class definition

#endif // !__CTCPCLIENT_H__

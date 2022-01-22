#ifndef __CTCPCLIENT_H__
#define __CTCPCLIENT_H__

#include <string>
#include "CCommunication.h"

class CTCPclient : CCommunication
{
public:
	CTCPclient(std::string host, int port);
	~CTCPclient();

	std::string getHost(void) const;
	int getPort(void) const;

protected:
	int recvFunc(std::string &msg);
	int sendFunc(std::string msg);

private:
	int sockfd;
	std::string host;
	int port;
};
// End CTCPclient class definition

#endif // !__CTCPCLIENT_H__

#ifndef __RCGATEWAY_H__
#define __RCGATEWAY_H__

#include "CRemoteClient.h"

class RCGateway : public CRemoteClient
{
public:
	RCGateway(int sd, MYSQL* database);

private:
	// Callbacks for Gateway command list
	static int lampCb(int, char *[]);
	static int parkCb(int, char *[]);
	static int idCb(int, char *[]);
	static int crqCb(int, char *[]);

private:
	static RCGateway* thisPtr;
};

#endif // !__RCGATEWAY_H__

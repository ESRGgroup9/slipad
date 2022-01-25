#ifndef __RCGATEWAY_H__
#define __RCGATEWAY_H__

#include "CRemoteClient.h"
#include "parser.h"

class RCGateway : public CRemoteClient
{
public:
	RCGateway(int sd);

private:
	// Callbacks for Gateway command list
	static int lampGatCb(int, char *[]);
	static int parkGatCb(int, char *[]);
	static int idGatCb(int, char *[]);
	static int crqGatCb(int, char *[]);

private:

};

#endif // !__RCGATEWAY_H__

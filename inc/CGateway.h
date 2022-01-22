#ifndef __CGATEWAY_H__
#define __CGATEWAY_H__

#include "CLoraComm.h"
#include "CTCPclient.h"

class CGateway
{
public:
	CGateway();
	~CGateway();

	void run();

private:
	static void *tLoraRecv(void*);
	static void *tTCPRecv(void*);

private:
	// CLoraComm localSysList;
	// CLoraComm localSys;
	// CTCPclient tcpclient;

	// queue<string> connReqList;
	// semaphore semConnReq;
};
// End CGateway class definition

#endif // !__CGATEWAY_H__

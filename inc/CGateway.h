#ifndef __CGATEWAY_H__
#define __CGATEWAY_H__

#include "CLoraComm.h"
#include "CTCPclient.h"
#include <pthread.h>

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

	pthread_t tLoraRecv_id;
	pthread_t tTCPRecv_id;

	// queue<string> connReqList;
	// semaphore semConnReq;
};
// End CGateway class definition

#endif // !__CGATEWAY_H__

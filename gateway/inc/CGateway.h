#ifndef __CGATEWAY_H__
#define __CGATEWAY_H__

#include "CLoraComm.h"
#include "CTCPclient.h"
// #include "parser.h"
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
	CLoraComm lora;
	CTCPclient tcp;

	pthread_t tLoraRecv_id;
	pthread_t tTCPRecv_id;

	// Parser tcpParser;
};
// End CGateway class definition

#endif // !__CGATEWAY_H__

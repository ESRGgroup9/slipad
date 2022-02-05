#ifndef __CGATEWAY_H__
#define __CGATEWAY_H__

#include "CLoraComm.h"
#include "CTCPclient.h"
#include <pthread.h>
#include <string>

class CGateway
{
public:
	CGateway(string host, int port);
	~CGateway();

	void run();

private:
	static void *tLoraRecv(void*);
	static void *tTCPRecv(void*);
	void connect();

private:
	CLoraComm lora;
	CTCPclient *tcp;

	pthread_t tLoraRecv_id;
	pthread_t tTCPRecv_id;

	std::string tcphost;
	int tcpport;
};
// End CGateway class definition

#endif // !__CGATEWAY_H__

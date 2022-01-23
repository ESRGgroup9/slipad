#include "CGateway.h"
#include "utils.h"
#include "debug.h"
#include <string>
using namespace std;

#define GATEWAY_ADDR 	(uint8_t)(0xcc)	// destination address
#define LS_ADDR			(uint8_t)(0xbb) // local address

#define TCP_HOST		("localhost")
#define TCP_PORT 		(5000)

CGateway::CGateway() :
	lora(433, LS_ADDR, GATEWAY_ADDR),
	tcp(TCP_HOST, TCP_PORT),
	tcpParser(NULL, " ")
{
	if(pthread_create(&tLoraRecv_id, NULL, tLoraRecv, this) != 0)
		panic("CGateway::CGateway(): pthread_create");

	if(pthread_create(&tTCPRecv_id, NULL, tTCPRecv, this) != 0)
		panic("CGateway::CGateway(): pthread_create");
}

CGateway::~CGateway()
{

}

void CGateway::run()
{

	pthread_join(tLoraRecv_id, NULL);
	pthread_join(tTCPRecv_id, NULL);
}

void *CGateway::tLoraRecv(void *arg)
{
	// get CGateway instance
	CGateway *c = reinterpret_cast<CGateway*>(arg);
	string msg;
	int err = 0;

	while(c)
	{
		err = c->lora.recv(msg);

		if(err == static_cast<int>(LoRaError::MSGOK))
		{
			DEBUG_MSG("[CGateway::tLoraRecv] Received[" << msg << "]");
			c->tcp.push(msg);
		}
	}

	DEBUG_MSG("[CGateway::tLoraRecv] exiting thread");
	return NULL;
}

void *CGateway::tTCPRecv(void *arg)
{
	// get CGateway instance
	CGateway *c = reinterpret_cast<CGateway*>(arg);
	string msg;
	int err = 0;

	while(c)
	{
		err = c->tcp.recv(msg);

		if(err == 0)
		{
			DEBUG_MSG("[CGateway::tLoraRecv] Received from TCP [" << msg << "]");
			c->tcpParser.parse(msg.c_str());
			// |-> returns cmd
			string cmd;
			c->lora.push(cmd);
		}
	}

	DEBUG_MSG("[CGateway::tTCPRecv] exiting thread");
	return NULL;
}

#include "CRemoteClient.h"
#include "utils.h"
#include "debug.h"

#include <netdb.h>
#include <sys/socket.h> 
#include <sys/types.h>
#include <unistd.h>
using namespace std;

CRemoteClient::CRemoteClient(int sd) :
	sockfd(sd)
{

}

CRemoteClient::~CRemoteClient()
{

}

void CRemoteClient::init(int recvPrio, int sendPrio)
{
	if(pthread_create(&tRecv_id, NULL, tRecv, this) != 0)
		panic("CRemoteClient::initThFunc(): pthread_create");

	CCommunication::init(sendPrio);
}

void CRemoteClient::run()
{
	pthread_join(tRecv_id, NULL);
	CCommunication::run();
}

void *CRemoteClient::tRecv(void *arg)
{
	// get CRemoteClient instance
	CRemoteClient *c = reinterpret_cast<CRemoteClient*>(arg);
	string msg;
	int err = 0;

	DEBUG_MSG("[CRemoteClient::tRecv] entering...");

	while(c)
	{
		err = c->CCommunication::recv(msg);

		if(err == 0)
		{
			DEBUG_MSG("[CRemoteClient::tRecv] Received[" << msg << "]");
			// parse
			//c->parser.parse(msg);
			c->CCommunication::push(msg);
		}
	}

	return NULL;
}

int CRemoteClient::recvFunc(string &msg)
{
	int err = 0;
	char buffer[128];

	// recv message from server
	err = ::recv(sockfd, &buffer, sizeof(buffer), 0);
	if(err == -1)
	{
		err = errno;

		// error caused by the lack of messages to read?
		if(err != EAGAIN)
		{
			// DEBUG_MSG("[CTCPclient::recvFunc] in recv()  - errno[" << err << "]");
		}
	}
	else
	{
		DEBUG_MSG("[CRemoteClient::recvFunc] Received[" << buffer << "]");
		// else, return the number of bytes read
		// copy received message to msg
		msg = string(buffer);
	}
	
	return err;
}

int CRemoteClient::sendFunc(string msg)
{
	int err = 0;

	// send message to server
	err = ::send(sockfd, msg.c_str(), msg.length(), 0);
	if(err == -1)
	{
		err = errno;
		ERROR_MSG("[CTCPclient::sendFunc] in send() - errno[" << err << "]");

		return -1;
	}
	// else, return the number of bytes sent
	return err;
}

string CRemoteClient::executeCmd(string cmd)
{

	return "query";
}
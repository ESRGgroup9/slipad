#include "CRemoteClient.h"
#include "utils.h"
#include "debug.h"

#include <cstring>
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
	// detach from tRecv and from CCommunication threads
	pthread_detach(tRecv_id);
	CCommunication::run(RUN_NONBLOCK);
}

void *CRemoteClient::tRecv(void *arg)
{
	// get CRemoteClient instance
	CRemoteClient *c = reinterpret_cast<CRemoteClient*>(arg);
	string msg;
	int ret = 0;

	while(c)
	{
		ret = c->CCommunication::recv(msg);

		if(ret == 0)
		{
			DEBUG_MSG("[CRemoteClient::tRecv] Stream socket peer[" << c->sockfd << "] has performed an orderly shutdown");
			break;
		}

		if(ret > 0)
		{
			DEBUG_MSG("[CRemoteClient::tRecv] Received[" << msg << "]");
			// parse
			// c->parser.parse(msg);
			// c->CCommunication::push(msg);
		}
	}

	return NULL;
}

int CRemoteClient::recvFunc(string &msg)
{
	int ret = 0;
	char buffer[256];

	// recv message from server
	ret = ::recv(sockfd, buffer, sizeof(buffer), 0);
	if(ret == -1)
	{
		ERROR_MSG("[CTCPclient::recvFunc] return -1: " << string(strerror(errno)));
	}
	else if(ret == 0)
	{
		// DEBUG_MSG("[CTCPclient::recvFunc] return 0: Stream socket peer has performed an orderly shutdown");
	}
	// else, return the number of bytes read
	else if(ret > 0)
	{
		// place null character at end of string
		buffer[ret] = '\0';
		// copy received message to msg
		msg = string(buffer);
		// DEBUG_MSG("[CTCPclient::recvFunc] return " << ret << ": Received [" << msg << "]");
	}

	return ret;
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
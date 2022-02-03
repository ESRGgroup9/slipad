#include "CTCPcomm.h"
#include "debug.h"

#include <sys/socket.h> // recv, send

using namespace std;

CTCPComm::CTCPComm(int sd)
{
	// initialize as non valid file descriptor
	setSockfd(sd);
	
	// set status
	this->status = ConnStatus::ONLINE;
}

void CTCPComm::setSockfd(int sd)
{
	this->sockfd = sd;
}

int CTCPComm::recvFunc(std::string &msg)
{
	int ret = 0;
	char buffer[256];

	// recv message from server
	ret = ::recv(sockfd, buffer, sizeof(buffer), MSG_DONTWAIT);
	if(ret > 0)
	{
		// place null character at end of string
		buffer[ret] = '\0';
		// copy received message to msg
		msg = string(buffer);
	}
	else if(ret == 0)
	{
		CCommunication::status = ConnStatus::CLOSED;
		// signal tSend in order to exit the thread
		pthread_cond_signal(&this->condtSend);
	}

	return ret;
}

int CTCPComm::sendFunc(std::string msg)
{
	int err = 0;

	DEBUG_MSG("[CTCPComm::sendFunc] Sending "<< msg << " ...");

	// send message to server
	// err = ::send(sockfd, msg.c_str(), msg.size(), MSG_DONTWAIT);
	err = ::send(sockfd, msg.c_str(), msg.length()+1, MSG_DONTWAIT);
	if(err == -1)
	{
		err = errno;
		ERROR_MSG("[CTCPComm::sendFunc] " << string(strerror(err)));
	}
	// else, return the number of bytes sent
	return err;
}
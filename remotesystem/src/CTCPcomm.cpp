#include "CTCPcomm.h"
#include "debug.h"

#include <sys/socket.h> // recv, send

using namespace std;

CTCPComm::CTCPComm(int sd)
{
	// initialize as non valid file descriptor
	this->sockfd = sd;
	
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
	ret = ::recv(sockfd, buffer, sizeof(buffer), 0);
	if(ret == -1)
	{
		ERROR_MSG("[CTCPComm::recvFunc] return -1: " << string(strerror(errno)));
	}
	else if(ret == 0)
	{
		// DEBUG_MSG("[CTCPComm::recvFunc] return 0: Stream socket peer has performed an orderly shutdown");
	}
	// else, return the number of bytes read
	else if(ret > 0)
	{
		// place null character at end of string
		buffer[ret] = '\0';
		// copy received message to msg
		msg = string(buffer);
		// DEBUG_MSG("[CTCPComm::recvFunc] return " << ret << ": Received [" << msg << "]");
	}

	return ret;
}

int CTCPComm::sendFunc(std::string msg)
{
	int err = 0;

	// send message to server
	err = ::send(sockfd, msg.c_str(), msg.size(), 0);
	if(err == -1)
	{
		err = errno;
		ERROR_MSG("[CTCPComm::sendFunc] return -1: " << string(strerror(errno)));
	}
	// else, return the number of bytes sent
	return err;
}
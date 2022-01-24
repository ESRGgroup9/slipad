
#define DEBUG

#include "CTCPserver.h"
#include "debug.h"
#include <string>
#include <iostream>

#include <netdb.h>
#include <sys/socket.h> 
#include <sys/types.h>
#include <unistd.h>
#include <cstring>
using namespace std;

static int myrecv(int sd);
static int mysend(int sd, string msg);

int main(int argc, char *argv[])
{
	if(argc < 2)
	{
		cout << "Usage: " << argv[0] << " <port>" << endl;
		return 1;
	}

	int port = atoi(argv[1]);
	CTCPserver server(port);
	int sd = 0;

	while(1)
	{
		sd = server.accept();

		if(sd != -1)
		{
			int ret = -1;
			do
			{
				ret = myrecv(sd);
				mysend(sd, "hello back ... ");
				for(int i = 0; i < 10000; i++)
					;
				mysend(sd, "motherfucker");
			}
			while(ret > 0);
		}
	}

	return 0;
}

static int myrecv(int sd)
{
	int ret = 0;
	char buffer[256];
	string msg;

	// recv message from server
	ret = ::recv(sd, buffer, sizeof(buffer), 0);
	if(ret == -1)
	{
		ERROR_MSG("[CTCPclient::recvFunc] return -1: " << string(strerror(errno)));
	}
	else if(ret == 0)
	{
		DEBUG_MSG("[CTCPclient::recvFunc] return 0: Stream socket peer has performed an orderly shutdown");
	}
	// else, return the number of bytes read
	else if(ret > 0)
	{
		// place null character at end of string
		buffer[ret] = '\0';
		// copy received message to msg
		msg = string(buffer);
		DEBUG_MSG("[CTCPclient::recvFunc] return " << ret << ": Received [" << msg << "]");
	}

	return ret;
}

static int mysend(int sd, string msg)
{
	int err = 0;

	// send message to server
	err = ::send(sd, msg.c_str(), msg.size(), 0);
	if(err == -1)
	{
		err = errno;
		ERROR_MSG("[CTCPclient::sendFunc] return -1: " << string(strerror(errno)));
	}
	// else, return the number of bytes sent
	return err;
}
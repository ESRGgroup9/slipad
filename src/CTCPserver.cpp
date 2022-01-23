#include "CTCPserver.h"
#include "utils.h"
#include "debug.h"

#include <cstring>

#include <netdb.h>
#include <sys/socket.h> 
#include <sys/types.h>
#include <unistd.h>

const int CTCPserver::maxNumClients = 5;

CTCPserver::CTCPserver(int port)
{
	this->port = port;
	// create tcp socket using IPv4 and connection-based byte stream
	this->listenSd = socket(PF_INET, SOCK_STREAM, 0);
	
	if (listenSd < 0)
		// socket file descriptor not created
		panic("socket");

	// bind port/addr to socket 
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	// Translate an unsigned short integer into network byte order
	addr.sin_port = htons(port);;
	// any interface
	addr.sin_addr.s_addr = INADDR_ANY;
	
	if(bind(listenSd, (struct sockaddr*)&addr, sizeof(addr)) != 0)
		panic("bind");

	// make listener with a maximum number of client slots
	if(listen(listenSd, maxNumClients) != 0)
		panic("listen")

	// get server host name
	// char host_name[256];
	// gethostname(host_name, sizeof(host_name));
	// printf("Starting server on %s:%d\n", host_name, atoi(argv[1]));
	// printf("Listening for incoming connections on sockfd:%d\n", listenSd);
}

CTCPserver::~CTCPserver()
{

}

int CTCPserver::accept()
{
	int n = sizeof(addr);
	// accept connections
	int sd = accept(listen_sd, (struct sockaddr*)&addr, &n);

	if((sd != -1) && (num_clients < MAX_CLIENT_NUM))
	{
		pthread_t child_recv;

		socket_table[num_clients].sockfd = sd;
		socket_table[num_clients].state = CLIENT_ALIVE;
		socket_table[num_clients].index = num_clients;

		// send ID to new client(index in socket table)
		char msg[16];
		// send ID in 'ID' field
		snprintf(msg, sizeof(msg), "ID;%d", num_clients);
		send(socket_table[num_clients].sockfd, msg, strlen(msg)+1, 0);

		printf("Client(%d) connected (%d slots available)\n", sd, MAX_CLIENT_NUM - (num_clients+1));
		// create thread to receive this socket's information
		pthread_create(&child_recv, 0, thread_recv, &socket_table[num_clients]);
		num_clients++;

		// dont track this
		pthread_detach(child_recv);
	} else
		printf("Cannot accept new connection\n");
		
	return 0;
}

/**
 * @file CTCPserver.h
 * @author Tomas Abreu, Diogo Fernandes
 * @date 1 fev 2022
 *
 * @brief Defines a TCP server with a maximum number of connected clients defined
 * in maxNumClients
 * 
 * Detailed Description
 */
#ifndef __CTCPSERVER_H__
#define __CTCPSERVER_H__

#include <netdb.h> // sockaddr_in

class CTCPserver
{
public:
/**
 * @brief Create TCP server on a given port
 * @param port - port to listen for new connections
 * @return none
 */
	CTCPserver(int port);
	~CTCPserver();

/**
 * @brief Accept new connection
 * @return int - socket file descriptor of new connection
 */
	int accept();

public:
	static int numClients;

private:
	static const int maxNumClients;
	
	int listenSd;
	int port;
	struct sockaddr_in addr;
};

#endif // !__CTCPSERVER_H__

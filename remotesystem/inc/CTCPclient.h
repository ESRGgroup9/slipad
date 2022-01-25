/**
 * @file CTCPclient.h
 * @author Tomas Abreu, Diogo Fernandes
 * @date 25 jan 2022
 *
 * @brief Implements a TCP client making use of CTCPComm to use TCP communication
 */
#ifndef __CTCPCLIENT_H__
#define __CTCPCLIENT_H__

#include "CTCPcomm.h"
#include <string>
#include <netdb.h> // sockaddr_in

class CTCPclient
{
public:
/**
 * @brief Creates a TCP client
 * @param host - hostname to connect to
 * @param port - port to connect to
 * @return none
 *
 * This only sets all necessary variables to connect to the given server info.
 * In order to connect, there is a method for that - 'connect'
 */
	CTCPclient(std::string host, int port);
/**
 * @brief Close the TCP connection
 */
	~CTCPclient();

/**
 * @brief Connect to the TCP server defined in CTCPclient constructor
 * @return 0 if connection successfull. -1 if not, with errno indicating the error.
 */
	int connect();

    // inline functions - uses functions from CTCPComm
    inline int recv(std::string &msg) { return tcp.recv(msg); }
    inline int send(std::string msg)  { return tcp.send(msg); }
    inline void push(std::string msg) { tcp.push(msg); }

/**
 * @brief Get host name
 */
	std::string getHost(void) const;
/**
 * @brief Get port number
 */
	int getPort(void) const;

private:
	CTCPComm tcp;

	int sockfd;
	int port;
	std::string host;
	struct sockaddr_in addr;
};
// End CTCPclient class definition

#endif // !__CTCPCLIENT_H__

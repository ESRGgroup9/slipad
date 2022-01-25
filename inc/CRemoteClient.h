/**
 * @file CRemoteClient.h
 * @author Tomas Abreu, Diogo Fernandes
 * @date 25 jan 2022
 *
 * @brief Implements a remote client, making use of CTCPComm to use TCP communication.
 * 
 * This class is used in CRemoteSystem as a result of a new CTCPserver connection.
 */
#ifndef __CREMOTECLIENT_H__
#define __CREMOTECLIENT_H__

#include "CCommunication.h" // ConnStatus
#include "CTCPcomm.h"
#include "parser.h"
#include <string>
// #include <vector>
#include <pthread.h>

// Defines the client type for this application
enum class ClientType
{
	UNDEF = -1,
	GATEWAY,
	WEBSITE,
	APPLICATION
};

class CRemoteClient
{
	// Defines all the information about the remote client
	struct ClientSocketInfo
	{
		enum ConnStatus state;
		// int index;
		// std::string name;
		int sockfd;
		enum ClientType type;
	};

public:
/**
 * @brief Defines the socket file descriptor to be used in TCP comms, as well
 * as initializes the client info.
 * @param sd - socket file descriptor
 * @return none
 */
	CRemoteClient(int sd);
	~CRemoteClient();

/**
 * @brief Create all threads used in communication
 * @param recvPrio - priority of receiving thread (tRecv)
 * @param sendPrio - priority of sending thread (CCommunication::tSend)
 * @return none
 */
	void init(int recvPrio, int sendPrio);

/**
 * @brief Detach from all threads, updating client status to ONLINE
 * @return none
 */
	void run();

protected:
	static void *tRecv(void *arg);

private:
	std::string executeCmd(std::string cmd);

private:
	CTCPComm tcp;
	ClientSocketInfo info;
	pthread_t tRecv_id;
	
	Parser clientParser;
	// std::vector<clientCmd*> cmdList;
};

#endif // !__CREMOTECLIENT_H__

/**
 * @file CRemoteClient.h
 * @author Tomas Abreu, Diogo Fernandes
 * @date 25 jan 2022
 *
 * @brief Implements a remote client, using TCP communication
 * 
 * This class is used in CRemoteSystem as a result of a new CTCPserver connection.
 */
#ifndef __CREMOTECLIENT_H__
#define __CREMOTECLIENT_H__

#include "CCommunication.h" // ConnStatus
#include "CTCPcomm.h"
#include <string>
// #include <vector>
#include <pthread.h>

// Defines the remote client type
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
		int index;
		std::string name;
		int sockfd;
		enum ClientType type;
	};

public:
	CRemoteClient(int sd);
	~CRemoteClient();

	void init(int recvPrio, int sendPrio);
	void run();

protected:
	static void *tRecv(void *arg);

private:
	std::string executeCmd(std::string cmd);

private:
	CTCPComm tcp;
	ClientSocketInfo info;
	// int sockfd;
	pthread_t tRecv_id;
	
	// std::vectir<clientCmd*> cmdList;
};

#endif // !__CREMOTECLIENT_H__

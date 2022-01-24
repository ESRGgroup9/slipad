#ifndef __CREMOTECLIENT_H__
#define __CREMOTECLIENT_H__

#include "CCommunication.h"
#include <string>
// #include <vector>
#include <pthread.h>

class CRemoteClient : public CCommunication
{
	enum class clientType
	{
		GATEWAY,
		WEBSITE,
		APPLICATION
	};

	struct clientSocketInfo
	{
		int state;
		int index;
		std::string name;
		int sockfd;
		enum clientType type;
	};

public:
	CRemoteClient(int sd);
	~CRemoteClient();

	void init(int recvPrio, int sendPrio);
	void run();

protected:
	static void *tRecv(void *arg);

	virtual int recvFunc(std::string &msg);
	virtual int sendFunc(std::string msg);

private:
	std::string executeCmd(std::string cmd);

private:
	int sockfd;
	pthread_t tRecv_id;
	clientSocketInfo info;
	// std::vectir<clientCmd*> cmdList;
};

#endif // !__CREMOTECLIENT_H__

#ifndef __CREMOTESYSTEM_H__
#define __CREMOTESYSTEM_H__

#include "CTCPserver.h"
#include "CRemoteClient.h"
#include "parser.h"
#include <mysql/mysql.h>
#include <vector>

class CRemoteSystem
{
public:
	CRemoteSystem(int port);
	~CRemoteSystem();
	
	void run();

private:
	static void *recvType(void *arg);
	static int typeCb(int argc, char *argv[]);

	void checkClientConnection();

private:
	static CRemoteSystem *thisPtr;
	Parser typeParser;
	Command_t *cmdList;

private:
	CTCPserver server;
	int client_port;
	std::vector <CRemoteClient*> clientList;
	MYSQL* db;
};

#endif // ! __CREMOTESYSTEM_H__

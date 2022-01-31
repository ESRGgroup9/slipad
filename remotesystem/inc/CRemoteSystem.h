#ifndef __CREMOTESYSTEM_H__
#define __CREMOTESYSTEM_H__

#include "CTCPserver.h"
#include "CRemoteClient.h"
#include "parser.h"
#include "timer.h"

#include <mysql/mysql.h>
#include <vector>
#include <signal.h>
#include <pthread.h>

class CRemoteSystem
{
public:
	CRemoteSystem(int port);
	~CRemoteSystem();
	
	void run();

private:
	static void *recvType(void *arg);
	static int typeCb(int argc, char *argv[]);

	static void timHandler(union sigval arg);
	void timCheckConnISR();

private:
	static CRemoteSystem *thisPtr;
	Parser typeParser;
	Command_t *cmdList;

private:
	CTCPserver server;
	int client_port;
	std::vector <CRemoteClient*> clientList;
	MYSQL* db;
	Timer timCheckConn;
	pthread_mutex_t mutAddClient;

};

#endif // ! __CREMOTESYSTEM_H__

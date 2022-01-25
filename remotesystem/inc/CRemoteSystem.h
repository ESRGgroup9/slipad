#ifndef __CREMOTESYSTEM_H__
#define __CREMOTESYSTEM_H__

#include "CTCPserver.h"
#include "CRemoteClient.h"
#include <mysql/mysql.h>
#include <vector>

class CRemoteSystem
{
public:
	// CRemoteSystem();
	CRemoteSystem(int port);
	void run();

private:
	CTCPserver server;
	std::vector <CRemoteClient*> clientList;
	MYSQL* db;
};

#endif // ! __CREMOTESYSTEM_H__

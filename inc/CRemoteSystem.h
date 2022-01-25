#ifndef __CREMOTESYSTEM_H__
#define __CREMOTESYSTEM_H__

#include "CTCPserver.h"
#include "CRemoteClient.h"
#include <vector>

class CRemoteSystem
{
public:
	// CRemoteSystem();
	CRemoteSystem(int sd);
	void run();

private:
	CTCPserver server;
	std::vector <CRemoteClient*> clientList;
	// CDatabase db;
};

#endif // ! __CREMOTESYSTEM_H__

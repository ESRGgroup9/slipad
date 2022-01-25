#include "CRemoteSystem.h"
#include "debug.h"

#define SERVER_PORT (5000)

CRemoteSystem::CRemoteSystem(int port) :
	server(port)
// CRemoteSystem::CRemoteSystem() :
	// server(SERVER_PORT)
{

}

void CRemoteSystem::run()
{
	int sd;

	DEBUG_MSG("[CRemoteSystem::run] Listening for new connections...");

	while(1)
	{
		sd = server.accept();

		if(sd != -1)
		{
			CRemoteClient *client = new CRemoteClient(sd);
			// client index
			static int i = 0;
			// DEBUG_MSG("[CRemoteSystem::run] client[" << i << "] on sockfd[" << sd << "]");
			
			// add new client
			clientList.push_back(client);
			// execute respective init and run methods
			clientList[i]->init(1,2);
			clientList[i]->run();
			i++;
			
			DEBUG_MSG("[CRemoteSystem::run] continue listening for new connections...");
		}
	}
}

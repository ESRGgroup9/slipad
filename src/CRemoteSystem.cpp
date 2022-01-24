#include "CRemoteSystem.h"
#include "debug.h"

#define SERVER_PORT (5000)

CRemoteSystem::CRemoteSystem(int sd) :
	server(sd)
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
			DEBUG_MSG("[CRemoteSystem::run] client[" << clientList.size() << "] on sockfd[" << sd << "]");
			CRemoteClient client(sd);
			// clientList.push_back(client);

			client.init(1,2);
			client.run();
		}
	}
}

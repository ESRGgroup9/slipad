#include "CRemoteSystem.h"
#include "RCGateway.h"
#include "utils.h"
#include "debug.h"
#include <iostream>
using namespace std;

#define SERVER_PORT (5000)

#define HOST		("localhost")
#define USER		("root")
#define PASSWORD	("Password123#@!")
#define DATABASE 	("slipad")

CRemoteSystem::CRemoteSystem(int port) :
	server(port)
// CRemoteSystem::CRemoteSystem() :
	// server(SERVER_PORT)
{
	db = mysql_init(NULL);
	if(!db)
		panic("MySQL: initialization failed");

	db = mysql_real_connect(db, HOST, USER, PASSWORD, DATABASE, 0, NULL, 0);
	if(!db)
	{
		cout << mysql_error(db) << endl;
		panic("MySQL: Connection Error");
	}

	if(mysql_query(db, "show tables") == 0)
	{
		cout << "show tables ok" << endl;
	}

	DEBUG_MSG("[CRemoteSystem] Successfully initialized!");
}

CRemoteSystem::~CRemoteSystem()
{
	mysql_close(db);
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
			// CRemoteClient *client = new CRemoteClient(sd);
			RCGateway *client = new RCGateway(sd, db);
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

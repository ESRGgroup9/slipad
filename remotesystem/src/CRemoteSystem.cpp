#include "CRemoteSystem.h"

#include "RCGateway.h"
#include "RCApplication.h"
#include "RCWebsite.h"
#include "CTCPcomm.h"

#include "utils.h"
#include "debug.h"
#include <iostream>
#include <pthread.h>

using namespace std;

#define SERVER_PORT (5000)
#define HOST		("localhost")
#define USER		("root")
#define PASSWORD	("Password123#@!")
#define DATABASE 	("slipad")

// check client connections after TIM_CHECK_CONN seconds
#define TIM_CHECK_CONN (10)

CRemoteSystem *CRemoteSystem::thisPtr = NULL;

CRemoteSystem::CRemoteSystem(int port) :
	typeParser(NULL, ";"),
	server(port),
	timCheckConn(TIM_CHECK_CONN, timHandler)
{
	// only one command to determine which type is the newly connected remote client
	cmdList = new Command_t[2]
	{
		{"TYPE", typeCb},
		{0,0}
	};

	// update parser command list
	typeParser.setCmdList(cmdList);

	db = new MYSQL;
	mysql_init(db);

	if(!db)
		panic("MySQL: initialization failed");

	db = mysql_real_connect(db, HOST, USER, PASSWORD, DATABASE, 0, NULL, 0);
	if(!db)
	{
		cout << mysql_error(db) << endl;
		panic("MySQL: Connection Error");
	}

	this->client_port = port;
	thisPtr = this;
}

CRemoteSystem::~CRemoteSystem()
{
	// close database connection
	mysql_close(db);

	// delete allocated variables
	delete[] cmdList;
	delete db;

	// delete client list
	for(auto client : clientList)
   	{
    	delete client;
   	}
}

void CRemoteSystem::timCheckConnISR()
{
	// Declaring iterator to a vector
    vector<CRemoteClient*>::iterator it;
      
    // search for non connected clients (connection CLOSED)
    for (it = clientList.begin(); it < clientList.end(); it++)
    {
    	if((*it)->info.state == ConnStatus::CLOSED)
		{
			// client has disconnected
			DEBUG_MSG("[CRemoteSystem::checkConn] Removing client[" << (*it)->info.sockfd << "] with connection closed...");
			// remove it from the client list
			clientList.erase(it);
			server.numClients--;
		}
    }
}

void CRemoteSystem::timHandler(union sigval arg)
{
	if(!thisPtr)
		panic("CRemoteSystem::timHandler(): thisPtr not defined");

	int id = arg.sival_int;
	DEBUG_MSG("[CRemoteSystem::timHandler] handling timer[" << id << "] timeout...");

	// cannot do switch statement since tim*.id is not a compile time constant
	if(id == thisPtr->timCheckConn.id)
	{
		thisPtr->timCheckConnISR();
	}
	else
	{
		ERROR_MSG("[CRemoteSystem::timHandler] unexpected timer event");
	}
}

void CRemoteSystem::run()
{
	int sd;

	DEBUG_MSG("[CRemoteSystem::run] Listening for new connections...");
	timCheckConn.start();

	while(1)
	{
		sd = server.accept();

		if(sd != -1)
		{
			pthread_t recvType_id;
			// handle client addition to client list in thread
			client_port = sd;
			pthread_create(&recvType_id, NULL, recvType, this);
			pthread_detach(recvType_id);
			DEBUG_MSG("[CRemoteSystem::run] Continue listening for new connections...");
		}
	}
}

int CRemoteSystem::typeCb(int argc, char *argv[])
{
	if(argc != 2)
	{
		DEBUG_MSG("[CRemoteSystem::typeCb] Usage: TYPE;<clientType>");
		return -1;
	}

	ClientType type = static_cast<ClientType>(atoi(argv[1]));
	CRemoteClient *client = NULL;

	switch(type)
	{
		case ClientType::GATEWAY:
			client = new RCGateway(thisPtr->client_port, thisPtr->db);
			break;

		case ClientType::WEBSITE:
			client = new RCWebsite(thisPtr->client_port, thisPtr->db);
			break;

		case ClientType::APPLICATION:
			client = new RCApplication(thisPtr->client_port, thisPtr->db);
			break;

		default:
		// do not handle other types - client stays NULL
			break;
	}

	if(client == NULL)
	{
		DEBUG_MSG("[CRemoteSystem::typeCb] Client not created");
		return -1;
	}

	// add new client
	thisPtr->clientList.push_back(client); 
	// execute respective init and run methods
	int i = thisPtr->clientList.size() - 1;
	thisPtr->clientList[i]->init(1,2);
	thisPtr->clientList[i]->run();
	DEBUG_MSG("[CRemoteSystem::typeCb] Client of type(" << static_cast<int>(type) << ") created successfully");

	return 0;
}
 
void *CRemoteSystem::recvType(void *arg)
{
	// get CRemoteSystem instance
	CRemoteSystem *c = reinterpret_cast<CRemoteSystem*>(arg);
	CTCPComm tcp(c->client_port);

	string msg;
	int ret = 0;
	int err = -1;

	do
	{
		ret = tcp.recv(msg);

		if(ret == 0)
			// client has closed the connection
			return NULL;

		if(ret > 0)
		{
			// parse received string
			err = c->typeParser.parse(msg.c_str());
		}
	}
	while(err != 0);

	return NULL;
}
#include "CRemoteClient.h"
#include "utils.h"
#include "debug.h"

using namespace std;

CRemoteClient::CRemoteClient(int sd, MYSQL* database) :
	db(database),
	// cmdParser command list is defined in derived classes
	cmdParser(NULL,";")
{
	// use 'sd' socket file descriptor - in tcp communications
	tcp.setSockfd(sd);
	
	// define client info
	info.sockfd = sd;
	info.state = ConnStatus::CREATED;
	info.type = ClientType::UNDEF;
}

CRemoteClient::~CRemoteClient()
{

}

void CRemoteClient::init(int recvPrio, int sendPrio)
{
	// >>>>>>>>>>>>>>>>>>>>>>>> set recvPrio
	if(pthread_create(&tRecv_id, NULL, tRecv, this) != 0)
		panic("CRemoteClient::init(): pthread_create");

	tcp.init(sendPrio);
}

void CRemoteClient::run()
{
	// detach from tRecv
	pthread_detach(tRecv_id);
	// detach from CTCPcomm threads
	tcp.run(RUN_NONBLOCK);
	// update client state
	info.state = ConnStatus::ONLINE;
}

void *CRemoteClient::tRecv(void *arg)
{
	// get CRemoteClient instance
	CRemoteClient *c = reinterpret_cast<CRemoteClient*>(arg);
	string msg;
	int ret = 0;
	int err = 0;

	do
	{
		ret = c->tcp.recv(msg);

		if(ret == -1)
		{
			err = errno;

			// non blocking operation?
			if(err != EAGAIN)
			{
				// unexpected error
				ERROR_MSG("[CRemoteClient::tRecv] " << string(strerror(err)));
			}
		}
		else if(ret > 0)
		{
			// DEBUG_MSG("[CRemoteClient("<< c->info.sockfd << ")::tRecv] Received[" << msg << "]");
			// make sure that the client has already identified himself
			if(c->info.type == ClientType::UNDEF)
			{
				DEBUG_MSG("[CRemoteClient("<< c->info.sockfd << ")::tRecv] Client type is not defined");
				continue;
			}
 
			// parse received string
			err = c->cmdParser.parse(msg.c_str());
			if(err != 0)
			{
				DEBUG_MSG("[CRemoteClient("<< c->info.sockfd << ")::tRecv] Parsing [" << msg << "] - error["<< err << "]");
			}
		}
	}
	while(ret != 0);

	DEBUG_MSG("[CRemoteClient("<< c->info.sockfd << ")::tRecv] Connection closed");
	c->info.state = ConnStatus::CLOSED;
	return NULL;
}

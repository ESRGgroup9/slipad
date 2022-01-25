#include "CRemoteClient.h"
#include "utils.h"
#include "debug.h"

using namespace std;

/*******************************************************************************
 * Gateway Command list
 * ****************************************************************************/
// Callbacks for Gateway command list
static int lampGatCb(int, char *[]);
static int parkGatCb(int, char *[]);
static int idGatCb(int, char *[]);
static int crqGatCb(int, char *[]);

// Command list for gateway remote client
static Command_t gatewayCmdList[] =
{
	{"LAMP"	, lampGatCb},
	{"PARK"	, parkGatCb},
	{"ID"	, idGatCb},
	{"CRQ"	, crqGatCb},
	{0,0}
};

/*******************************************************************************
 * Website Command list
 * ****************************************************************************/
// static int (int, char *[]);
// static int (int, char *[]);

// Command list for website remote client
// static Command_t websiteCmdList[] =
// {
// 	// {"PARK", parkWebCb},
// 	{0,0}
// };

/*******************************************************************************
 * Application Command list
 * ****************************************************************************/
// static Command_t applicationCmdList[] =
// {
// 	{0,0}
// };

/*******************************************************************************
 * CRemoteClient methods
 * ****************************************************************************/
CRemoteClient::CRemoteClient(int sd) :
	clientParser(gatewayCmdList," ")
{
	// use 'sd' - socket file descriptor - in tcp communications
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
		panic("CRemoteClient::initThFunc(): pthread_create");

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

	while(c)
	{
		ret = c->tcp.recv(msg);

		if(ret == 0)
		{
			// client has closed the connection
			DEBUG_MSG("[CRemoteClient::tRecv] Stream socket peer[" << c->info.sockfd << "] has performed an orderly shutdown");
			c->info.state = ConnStatus::CLOSED;
			break;
		}

		if(ret > 0)
		{
			DEBUG_MSG("[CRemoteClient::tRecv] Received[" << msg << "]");
			// parse received string
			// c->clientParser.parse(msg.c_str());

			// make sure that the client has already identified himself
			if(c->info.type != ClientType::UNDEF)
				c->tcp.push(msg);
		}
	}

	return NULL;
}

string CRemoteClient::executeCmd(string cmd)
{

	return "query";
}

/*******************************************************************************
 * Gateway Command list callbacks
 * ****************************************************************************/
static int lampGatCb(int, char *[])
{

	return 0;
}

static int parkGatCb(int, char *[])
{

	return 0;
}

static int idGatCb(int, char *[])
{

	return 0;
}

static int crqGatCb(int, char *[])
{

	return 0;
}

/*******************************************************************************
 * Website Command list callbacks
 * ****************************************************************************/

/*******************************************************************************
 * Application Command list callbacks
 * ****************************************************************************/

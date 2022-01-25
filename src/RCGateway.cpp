#include "RCGateway.h"
#include "debug.h"

RCGateway::RCGateway(int sd) :
	CRemoteClient(sd)
{
	// Command list for gateway remote client
	cmdList = new Command_t[5]
	{
		{"LAMP"	, lampGatCb},
		{"PARK"	, parkGatCb},
		{"ID"	, idGatCb},
		{"CRQ"	, crqGatCb},
		{0,0}
	};

	// update parser comand list
	cmdParser.setCmdList(cmdList);

	// update remote client type
	info.type = ClientType::GATEWAY;
	DEBUG_MSG("[RCGateway] New GATEWAY client");
}

int RCGateway::lampGatCb(int argc, char *argv[])
{
	DEBUG_MSG("[RCGateway::lampGatCb] entering");
	return 0;
}

int RCGateway::parkGatCb(int argc, char *argv[])
{
	DEBUG_MSG("[RCGateway::parkGatCb] entering");
	return 0;
}

int RCGateway::idGatCb(int argc, char *argv[])
{
	DEBUG_MSG("[RCGateway::idGatCb] entering");
	return 0;
}

int RCGateway::crqGatCb(int argc, char *argv[])
{
	DEBUG_MSG("[RCGateway::crqGatCb] entering");
	return 0;
}


#include "RCWebsite.h"
#include "debug.h"

RCWebsite* RCWebsite::thisPtr = NULL;

RCWebsite::RCWebsite(int sd, MYSQL* database) :
	CRemoteClient(sd, database)
{
	// Command list for gateway remote client
	cmdList = new Command_t[4]
	{
		{0,0}
	};

	// update parser comand list
	cmdParser.setCmdList(cmdList);

	// update remote client type
	info.type = ClientType::WEBSITE;
	DEBUG_MSG("[RCApplication] New WEBSITE client");

	thisPtr = this;
}

RCWebsite::~RCWebsite()
{
	delete[] cmdList;
}
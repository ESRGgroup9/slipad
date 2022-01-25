#include "RCGateway.h"
#include "debug.h"
#include <mysql/mysql.h>
#include <sstream>
using namespace std;

RCGateway* RCGateway::thisPtr = NULL;

RCGateway::RCGateway(int sd, MYSQL* database) :
	CRemoteClient(sd, database)
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

	thisPtr = this;
}

int RCGateway::lampGatCb(int argc, char *argv[])
{
	DEBUG_MSG("[RCGateway::lampGatCb] entering");

	if(argc != 2)
		return -1;

	// check that command argument, argv[1], is valid
	// ....

	// execute query
	stringstream query;
	query << "UPDATE * FROM bank_account WHERE acc_no=" << argv[1];

	if(mysql_query(thisPtr->db, query.str().c_str()) == 0)
	{

	}

	return 0;
}

int RCGateway::parkGatCb(int argc, char *argv[])
{
	DEBUG_MSG("[RCGateway::parkGatCb] entering");

	if(argc != 2)
		return -1;
	
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


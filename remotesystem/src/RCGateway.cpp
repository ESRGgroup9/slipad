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

	// get lamppost ID
	int id = 1;
	// ....

	// execute query
	stringstream query;
	query << "UPDATE lamppost SET status='" << argv[1] << "' WHERE id=" << id;

	DEBUG_MSG("QUERY: " << query.str());

	if(mysql_query(thisPtr->db, query.str().c_str()) == 0)
	{
		DEBUG_MSG("UPDATE Lamppost[" << id << "] to status[" << argv[1] << "] successful");
	}
	else
	{
		DEBUG_MSG("UPDATE Lamppost[" << id << "] to status[" << argv[1] << "] failed");	
	}

	return 0;
}

int RCGateway::parkGatCb(int argc, char *argv[])
{
	DEBUG_MSG("[RCGateway::parkGatCb] entering");

	if(argc != 2)
		return -1;
	
	// MYSQL_RES* rset;
 //   	MYSQL_ROW row;

	// if(mysql_query(thisPtr->db, query.str().c_str()) == 0)
	// {
	// 	rset = mysql_use_result(thisPtr->db);
	// 	row = mysql_fetch_row(rset);

	// 	// b->setAccountNumber(atoi(row[0]));
	// 	// b->setFirstName(row[1]);
	// 	// b->setLastName(row[2]);
	// 	// b->setBalance(atof(row[3]));
	// }

	// mysql_free_result(rset);

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


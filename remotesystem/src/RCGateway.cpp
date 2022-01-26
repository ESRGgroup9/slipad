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
	if(argc != 3)
	{
		DEBUG_MSG("[RCGateway::lampGatCb] Usage: LAMP <status> <lampID>");
		// <status> can be: MIN | FAIL | ON | OFF
		return -1;
	}

	// get lamppost ID
	int id = atoi(argv[2]);

	// execute query
	stringstream query;
	query << "UPDATE lamppost SET status='" << argv[1] << "' WHERE id=" << id;
	DEBUG_MSG("[RCGateway::lampGatCb] " << query.str());

	if(mysql_query(thisPtr->db, query.str().c_str()) == 0)
	{
		DEBUG_MSG("[RCGateway::lampGatCb] Query OK");
	}
	else
	{
		DEBUG_MSG("[RCGateway::lampGatCb] Invalid status(" << argv[1] << ") or ID(" << id << ")");	
	}

	return 0;
}

int RCGateway::parkGatCb(int argc, char *argv[])
{
	if(argc != 3)
	{
		DEBUG_MSG("[RCGateway::parkGatCb] Usage: PARK <num_vacants> <lampID>");
		return -1;
	}

	// get lamppost ID
	int id = atoi(argv[2]);
	// get number of available parking spaces
	int num_vacants = atoi(argv[1]);

	// execute query
	stringstream query;
	query << "UPDATE parking_space SET num_vacants=" << num_vacants << " WHERE id=" << id;
	DEBUG_MSG("[RCGateway::parkGatCb] " << query.str());

	if(mysql_query(thisPtr->db, query.str().c_str()) == 0)
	{
		DEBUG_MSG("[RCGateway::parkGatCb] Query OK");
	}
	else
	{
		DEBUG_MSG("[RCGateway::parkGatCb] Invalid num_vacants(" << num_vacants << ") or ID(" << id << ")");	
	}

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


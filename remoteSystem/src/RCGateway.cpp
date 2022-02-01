#include "RCGateway.h"
#include "debug.h"
#include <sstream>
using namespace std;

RCGateway* RCGateway::thisPtr = NULL;

RCGateway::RCGateway(int sd, MYSQL* database) :
	CRemoteClient(sd, database)
{
	// Command list for gateway remote client
	cmdList = new Command_t[5]
	{
		{"LAMP"	, lampCb},
		{"PARK"	, parkCb},
		{"ID"	, idCb},
		{"CRQ"	, crqCb},
		{0,0}
	};

	// update parser comand list
	cmdParser.setCmdList(cmdList);

	// update remote client type
	info.type = ClientType::GATEWAY;
	DEBUG_MSG("[RCGateway] New GATEWAY client");

	thisPtr = this;
}

RCGateway::~RCGateway()
{
	delete[] cmdList;
}

int RCGateway::lampCb(int argc, char *argv[])
{
	if(argc != 3)
	{
		DEBUG_MSG("[RCGateway::lampCb] Usage: LAMP;<status>;<lampID>");
		// <status> can be: MIN | FAIL | ON | OFF
		return -1;
	}

	// get lamppost ID
	int id = atoi(argv[2]);

	// execute query
	stringstream query;
	query << "UPDATE lamppost SET status='" << argv[1] << "' WHERE id=" << id;
	DEBUG_MSG("[RCGateway::lampCb] " << query.str());

	if(mysql_query(thisPtr->db, query.str().c_str()) != 0)
	{
		DEBUG_MSG("[RCGateway::lampCb] Invalid status(" << argv[1] << ") or ID(" << id << ")");	
		return -1;
	}

	return 0;
}

int RCGateway::parkCb(int argc, char *argv[])
{
	if(argc != 3)
	{
		DEBUG_MSG("[RCGateway::parkCb] Usage: PARK;<num_vacants>;<lampID>");
		return -1;
	}

	// get lamppost ID
	int id = atoi(argv[2]);
	// get number of available parking spaces
	int num_vacants = atoi(argv[1]);

	// execute query
	stringstream query;
	query << "UPDATE parking_space SET num_vacants=" << num_vacants << " WHERE id=" << id;
	DEBUG_MSG("[RCGateway::parkCb] " << query.str());

	if(mysql_query(thisPtr->db, query.str().c_str()) != 0)
	{
		DEBUG_MSG("[RCGateway::parkCb] Invalid num_vacants(" << num_vacants << ") or ID(" << id << ")");	
		return -1;
	}

	return 0;
}

int RCGateway::crqCb(int argc, char *argv[])
{
	if(argc != 2)
	{
		DEBUG_MSG("[RCGateway::crqCb] Usage: CRQ;<lsLocalAddr>");
		return -1;
	}

	/* Lamppost was already created using RCApplication, so the lamppost ID and
	address are already defined in the database. However, the LS must give the 
	RS its local address, in order for this to inform the LS of its 'virtual'
	address, to be used in further communications. */

	// get lamppost Local Address
	int addr = atoi(argv[1]);
	stringstream query;

	// get lamppost ID from database
	query << "SELECT id FROM lamppost WHERE address=" << addr;
	DEBUG_MSG("[RCGateway::crqCb] " << query.str());
	// execute query
	if(mysql_query(thisPtr->db, query.str().c_str()) != 0)
	{
		DEBUG_MSG("[RCGateway::crqCb] Error: " << mysql_error(thisPtr->db));
		return -1;
	}

    // get the result set
    MYSQL_RES *res = mysql_store_result(thisPtr->db);

    // get the number of the columns
    int num_fields = mysql_num_fields(res);

    // get rows
    MYSQL_ROW row = mysql_fetch_row(res);

  	if(res != NULL)
    	mysql_free_result(res);

    // get lamppost ID
    int lamppost_id = -1;

   	if((num_fields == 1) && (row))
   	{
       	lamppost_id = atoi(row[0]);
       	DEBUG_MSG("[RCGateway::crqCb] LS with addr(" << addr << ") has ID(" << lamppost_id << ")");
   	}
   	else
   	{
   		DEBUG_MSG("[RCGateway::crqCb] Invalid addr(" << addr << ")");
   		return -1;
   	}

   	// clear query
	query.str("");

   	// define gateway sockfd for this lamppost
   	query << "UPDATE lamppost SET sockfd=" << thisPtr->info.sockfd;
   	query << " WHERE id=" << lamppost_id;

	DEBUG_MSG("[RCGateway::crqCb] " << query.str());
	// execute query
	if(mysql_query(thisPtr->db, query.str().c_str()) != 0)
	{
		DEBUG_MSG("[RCGateway::crqCb] Invalid sockfd("<< thisPtr->info.sockfd <<")");
		return -1;
	}

	// send ID to LS
	// ID command Usage: ID;<id>;<lsAddr>
	string msg = "ID;" + to_string(lamppost_id) + ";" + argv[1];
	// send command
    thisPtr->tcp.push(msg);

	return 0;
}

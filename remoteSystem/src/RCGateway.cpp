#include "RCGateway.h"
#include "debug.h"
#include <sstream>
using namespace std;

RCGateway* RCGateway::thisPtr = NULL;

RCGateway::RCGateway(int sd, MYSQL* database) :
	CRemoteClient(sd, database)
{
	// Command list for gateway remote client
	cmdList = new Command_t[4]
	{
		{"LAMP"	, lampCb},
		{"PARK"	, parkCb},
		{"CRQ"	, crqCb},
		{0,0}
	};

	// update parser comand list
	cmdParser.setCmdList(cmdList);

	// update remote client type
	info.type = ClientType::GATEWAY;
	DEBUG_MSG("[RCGateway] New GATEWAY client connected");

	thisPtr = this;
}

RCGateway::~RCGateway()
{
	delete[] cmdList;
}

int RCGateway::parkCb(int argc, char *argv[])
{
	if(argc != 3)
	{
		DEBUG_MSG("[RCGateway::parkCb] Usage: PARK;<num_vacants>;<lampID>");
		return -1;
	}

	// get number of available parking spaces
	int num_vacants = atoi(argv[1]);

	// get lamppost ID
	int id = atoi(argv[2]);

	// execute query
	stringstream query;
	query << "UPDATE parking_space SET num_vacants=" << num_vacants << " WHERE id=" << id;
	// DEBUG_MSG("[RCGateway::parkCb] " << query.str());

	if(mysql_query(thisPtr->db, query.str().c_str()) != 0)
	{
		DEBUG_MSG("[RCGateway::parkCb] Invalid num_vacants(" << num_vacants << ") or ID(" << id << ")");	
		return -1;
	}

	DEBUG_MSG("[RCGateway::parkCb] parking_space("<< id <<") has "<< num_vacants << " vacants");

	return 0;
}

int RCGateway::crqCb(int argc, char *argv[])
{
	if(argc != 2)
	{
		DEBUG_MSG("[RCGateway::crqCb] Usage: CRQ;<lsLocalAddr>");
		return -1;
	}

	// get lamppost Local Address
	int addr = atoi(argv[1]);
	stringstream query;

	// get lamppost ID from database
	query << "SELECT id FROM lamppost WHERE address=" << addr;
	// DEBUG_MSG("[RCGateway::crqCb] " << query.str());
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
   	int gateway_sd = thisPtr->info.sockfd;
   	query << "UPDATE lamppost SET gateway_sd=" << gateway_sd;
   	query << " WHERE id=" << lamppost_id;

	// DEBUG_MSG("[RCGateway::crqCb] " << query.str());
	// execute query
	if(mysql_query(thisPtr->db, query.str().c_str()) != 0)
	{
		DEBUG_MSG("[RCGateway::crqCb] Invalid gateway_sd("<< gateway_sd <<")");
		return -1;
	}

	// send ID to LS
	// ID command Usage: ID;<id>;<lsAddr>
	string msg = "ID;" + to_string(lamppost_id) + ";" + argv[1];
	// send command
    thisPtr->tcp.push(msg);
    // DEBUG_MSG("[RCGateway::crqCb] Sending cmd(" << msg << ") from sockfd(" << gateway_sd << ")");

	return 0;
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
	int lamppostId = atoi(argv[2]);

	// execute query
	stringstream query;
	query << "UPDATE lamppost SET status='" << argv[1] << "' WHERE id=" << lamppostId;
	// DEBUG_MSG("[RCGateway::lampCb] " << query.str());

	if(mysql_query(thisPtr->db, query.str().c_str()) != 0)
	{
		DEBUG_MSG("[RCGateway::lampCb] Invalid status(" << argv[1] << ") or ID(" << lamppostId << ")");	
		return -1;
	}

	DEBUG_MSG("[RCGateway::lampCb] lamppost("<< lamppostId << ") with status("<< argv[1] << ")");

	// check if the lamppost has turned on, i.e, status=ON
	if(strcmp(argv[1], "ON") == 0)
	{
		// lamppost has turned ON. Find neighbour lampposts to turn ON
		// turn neighbours ON
		// send command to turn them ON
		dynamicLightUp(lamppostId);
	}

	return 0;
}

void RCGateway::dynamicLightUp(int lamppostId)
{
	stringstream query;
	MYSQL_RES *res;
	MYSQL_ROW row;
	int num_fields;

	// get next lamppost
	int next_lamppostId = -1;
	query << "SELECT id FROM lamppost WHERE id=";
	query << "(SELECT MIN(id) FROM lamppost WHERE id >" << lamppostId << ")";
	// execute query
	mysql_query(thisPtr->db, query.str().c_str());
	{
		// get the result set
	    res = mysql_store_result(thisPtr->db);
	    // get the number of the columns
	    num_fields = mysql_num_fields(res);
	    // get rows
	    row = mysql_fetch_row(res);

	  	if(res != NULL)
	    	mysql_free_result(res);

	   	if((num_fields == 1) && (row))
	     	next_lamppostId = atoi(row[0]);
	}

	// clear query
	query.str("");

	// get previous lamppost
	int prev_lamppostId = -1;
	query << "SELECT id FROM lamppost WHERE id=";
	query << "(SELECT MAX(id) FROM lamppost WHERE id <" << lamppostId << ")";
	// execute query
	mysql_query(thisPtr->db, query.str().c_str());
	{
		// get the result set
	    res = mysql_store_result(thisPtr->db);
	    // get the number of the columns
	    num_fields = mysql_num_fields(res);
	    // get rows
	    row = mysql_fetch_row(res);

	  	if(res != NULL)
	    	mysql_free_result(res);

	   	if((num_fields == 1) && (row))
	     	prev_lamppostId = atoi(row[0]);
	}

	// clear query
	query.str("");

	// turn on those lampposts
	query << "UPDATE lamppost set status='ON' where id IN (";
	query << next_lamppostId << "," << prev_lamppostId << ")";
	mysql_query(thisPtr->db, query.str().c_str());
	// DEBUG_MSG("[RCGateway::dynamicLightUp] "<< query.str());

	// send request to turn on the lamp of next and prev lamppost
	string msg;

	msg = "LAMP;ON;" + to_string(prev_lamppostId);
    thisPtr->tcp.push(msg);
    // DEBUG_MSG("[RCGateway::dynamicLightUp] Sending cmd(" << msg << ") from sockfd(" << thisPtr->info.sockfd << ")");
	DEBUG_MSG("[RCGateway::dynamicLightUp] requesting lamppost("<< prev_lamppostId << ") ON");

	msg = "LAMP;ON;" + to_string(next_lamppostId);
    thisPtr->tcp.push(msg);
    // DEBUG_MSG("[RCGateway::dynamicLightUp] Sending cmd(" << msg << ") from sockfd(" << thisPtr->info.sockfd << ")");
    DEBUG_MSG("[RCGateway::dynamicLightUp] requesting lamppost("<< next_lamppostId << ") ON");
}

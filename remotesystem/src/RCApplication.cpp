#include "RCApplication.h"
#include "debug.h"
#include <sstream>
using namespace std;

RCApplication* RCApplication::thisPtr = NULL;

RCApplication::RCApplication(int sd, MYSQL* database) :
	CRemoteClient(sd, database)
{
	// Command list for gateway remote client
	cmdList = new Command_t[4]
	{
		{"ADD"		, addCb},
		{"MOD"		, modifyCb},
		{"CONSULT"	, consultCb},
		{0,0}
	};

	// update parser comand list
	cmdParser.setCmdList(cmdList);

	// update remote client type
	info.type = ClientType::APPLICATION;
	DEBUG_MSG("[RCApplication] New APPLICATION client");

	thisPtr = this;
}

int RCApplication::addCb(int argc, char *argv[])
{
	/*
	This command must perform:
	- (MAY) Add a new region - post_code, operatorID, parish, county, district
	- Add a new location - (ID) latitude, longitude, post_code, street_name
	- Add a new lamppost - (locationID) address, status
	- Create a parking_space (locationID)
	*/
	if(argc != 6)
	{
		DEBUG_MSG("[RCApplication::addCb] Usage: ADD <street_name> <post_code> <parish> <county> <district>");
		return -1; 
	}

	stringstream query;
	string post_code, parish, county, district;
	int operator_id;

	// operator: ID, name, password
	query << "INSERT INTO operator VALUES(";
	query << operator_id << ",";
	query << name << ",";
	query << password << ",";

	// region: post_code, operatorID, parish, county, district
	query << "INSERT INTO region VALUES(";
	query << post_code << ",";
	query << operator_id << ",";
	query << parish << ",";
	query << county << ",";
	query << district << ")";

	// location: id, latitude, longitude, post_code, street_name
	query << "INSERT INTO location VALUES(";
	query << locationID << ",";
	query << latitude << ",";
	query << longitude << ",";
	query << post_code << ",";
	query << street_name << ")";

	// lamppost: locationID, address, status
	query << "INSERT INTO lamppost VALUES(";
	query << locationID << ",";
	query << address << ",";
	query << status << ")";

	// parking_space: lamppostID, num_vacants
	query << "INSERT INTO parking_space  VALUES(";
	query << locationID << ",";
	query << address << ",";
	query << status << ")";

	DEBUG_MSG("[RCGateway::addCb] " << query.str());

	if(mysql_query(thisPtr->db, query.str().c_str()) == 0)
	{
		DEBUG_MSG("[RCGateway::addCb] Query OK");
	}
	else
	{
		// DEBUG_MSG("[RCGateway::addCb] Invalid status(" << argv[1] << ") or ID(" << id << ")");	
		return -1;
	}

	return 0;

	return 0;
}

int RCApplication::modifyCb(int argc, char *argv[])
{

	return 0;
}

int RCApplication::consultCb(int argc, char *argv[])
{

	return 0;
}
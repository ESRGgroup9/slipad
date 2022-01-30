#include "RCApplication.h"
#include "debug.h"
#include "CTCPcomm.h"

#include <sstream>
#include <stdexcept> // invalid_argument
using namespace std;

RCApplication* RCApplication::thisPtr = NULL;

RCApplication::RCApplication(int sd, MYSQL* database) :
	CRemoteClient(sd, database)
{
	// Command list for gateway remote client
	cmdList = new Command_t[6]
	{
		{"ADD"		, addCb},
		{"MOD"		, modifyCb},
		{"CONSULT"	, consultCb},
		{"SIGNIN"	, signInCb},
		{"SIGNUP"	, signUpCb},
		{0,0}
	};

	// update parser comand list
	cmdParser.setCmdList(cmdList);

	// update remote client type
	info.type = ClientType::APPLICATION;
	DEBUG_MSG("[RCApplication] New APPLICATION client");

	thisPtr = this;
}

RCApplication::~RCApplication()
{
	delete[] cmdList;
}

int RCApplication::modifyCb(int argc, char *argv[])
{
	if(argc != 2)
	{
		// DEBUG_MSG("[RCApplication::modifyCb] Usage: MOD <operator_id> <lamppost_id>");
		DEBUG_MSG("[RCApplication::modifyCb] Usage: MOD;<lamppost_id>");
		return -1;
	}

	stringstream query;
	int lamppost_id = atoi(argv[1]);

	query << "UPDATE lamppost SET status='OFF' WHERE ";
	query << "id=" << lamppost_id;

	// execute query
	DEBUG_MSG("[RCApplication::modifyCb] " << query.str());
	if(mysql_query(thisPtr->db, query.str().c_str()) != 0)
	{
		DEBUG_MSG("[RCApplication::modifyCb] Invalid lamppost id(" << lamppost_id << ")");
		return -1;
	}

	return 0;
}

int RCApplication::consultCb(int argc, char *argv[])
{
	if(argc != 2)
	{
		DEBUG_MSG("[RCApplication::consultCb] Usage: CONSULT;<post_code>");
		return -1;
	}
	
	stringstream query;

	// select p.id,p.status,l.post_code,l.street_name,r.parish,r.county,r.district,l.latitude,l.longitude
	// from lamppost p, location l, region r
	// where l.id=p.id and l.post_code=r.post_code and l.post_code=0;

	query << "SELECT p.id,p.status,l.post_code,l.street_name,r.parish,r.county,r.district ";
	query << "FROM lamppost p, location l, region r ";
	query << "WHERE l.id=p.id AND l.post_code=r.post_code;";

	// execute query
	DEBUG_MSG("[RCApplication::consultCb] " << query.str());
	if(mysql_query(thisPtr->db, query.str().c_str()) != 0)
	{
		DEBUG_MSG("[RCApplication::consultCb] Invalid consult: " << mysql_error(thisPtr->db));
		return -1;
	}

	int lamppost_id;
	string street_name;
	string post_code;
	string parish;
	string county;
	string district;
	string status;

    // get the result set
    MYSQL_RES *res = mysql_store_result(thisPtr->db);
    // get the number of the columns
	int num_fields = mysql_num_fields(res);
	MYSQL_ROW row;
    int err = 0;
    int num_lamppost = 0;

    if(num_fields != 7)
    {
    	DEBUG_MSG("[RCApplication::consultCb] Invalid result with " << num_fields << " columns instead of 7");
  		err = -1;
    }
    else
    {
    	CTCPComm tcp(thisPtr->info.sockfd);

    	while((row = mysql_fetch_row(res)))
	    {
	    	lamppost_id = atoi(row[0]);
	    	status 		= row[1];
	    	post_code 	= row[2];
	    	street_name = row[3];
	    	parish 		= row[4];
	    	county 		= row[5];
	    	district	= row[6];

	    	char str[256];
	    	snprintf(str, sizeof(str), "ID(%d) STATUS(%s) POST_CODE(%s) STREET(%s) PARISH(%s) COUNTY(%s) DISTRICT(%s)",
	    		lamppost_id, status.c_str(), post_code.c_str(), street_name.c_str(), parish.c_str(), county.c_str(), district.c_str());
	    	// DEBUG_MSG("[RCApplication::consultCb] " << string(str));
	    	
	    	// send lamppost info to the remote client
	    	// tcp.send(str);

	    	num_lamppost++;
	    }

	    DEBUG_MSG("[RCApplication::consultCb] Successfully returned " << num_lamppost << " lampposts registers");
	}

   	if(res != NULL)
    	mysql_free_result(res);

	return err;
}

int RCApplication::signInCb(int argc, char *argv[])
{
	if(argc != 4)
	{
		DEBUG_MSG("[RCApplication::signInCb] Usage: SIGNIN;<operator_id>;<name>;<password>");
		return -1;
	}
	
	stringstream query;
	int operator_id = atoi(argv[1]);
	query << "SELECT id FROM operator WHERE ";
	query << "id=" << operator_id << " AND ";
	query << "name='" << argv[2] << "' AND ";
	query << "password='" << argv[3] << "'";

	// execute query
	DEBUG_MSG("[RCApplication::signInCb] " << query.str());
	if(mysql_query(thisPtr->db, query.str().c_str()) != 0)
	{
		DEBUG_MSG("[RCApplication::signInCb] Invalid sign IN: " << mysql_error(thisPtr->db));
		return -1;
	}

	// just check if the query returned an empty set or not
	MYSQL_RES *res;
    MYSQL_ROW row;
    int err = 0;
    int num_fields = 0;

    // get the result set
    res = mysql_store_result(thisPtr->db);

    // get the number of the columns
    num_fields = mysql_num_fields(res);

   	if((num_fields == 1) && (row = mysql_fetch_row(res)))
   	{
		if(row[0] != NULL)
		{
        	// operator_id is the same as atoi(argv[1]), as declared before
        	operator_id = atoi(row[0]);
        	DEBUG_MSG("[RCApplication::signInCb] Operator with id(" << operator_id << ") signed IN");
		}
   	}
   	else
   	{
   		DEBUG_MSG("[RCApplication::signInCb] Invalid sign IN: operator("<< operator_id << ") credentials doesnt match");
   		err = -1;
   	}

   	if(res != NULL)
    	mysql_free_result(res);

	return err;
}

int RCApplication::signUpCb(int argc, char *argv[])
{
	if(argc != 3)
	{
		DEBUG_MSG("[RCApplication::signUpCb] Usage: SIGNUP;<name>;<password>");
		return -1;
	}
	
	stringstream query;
	// operator: ID, name, password
	query << "INSERT INTO operator VALUES(";
	query << "NULL" << ","; // operator ID is generated by auto increment
	// add Name
	query << "'" << argv[1] << "',";
	// add Password
	query << "'" << argv[2] << "')";

	// execute query
	DEBUG_MSG("[RCApplication::signUpCb] " << query.str());
	if(mysql_query(thisPtr->db, query.str().c_str()) != 0)
	{
		DEBUG_MSG("[RCApplication::signUpCb] Insert signUp: " << mysql_error(thisPtr->db));
		return -1;
	}

	// retrieve operator ID
	int operator_id = mysql_insert_id(thisPtr->db);
	DEBUG_MSG("[RCApplication::signUpCb] New operator has id(" << operator_id << ")");
	// ....

	return 0;
}

int RCApplication::addCb(int argc, char *argv[])
{
	if(argc != 10)
	{
		DEBUG_MSG("[RCApplication::addCb] Usage: ADD;<operator_id>;<addr>;<street_name>;<post_code>;<parish>;<county>;<district>;<latitude>;<longitude>");		return -1; 
	}
 
	// improve readability
	int operator_id		= atoi(argv[1]);
	int address 		= atoi(argv[2]);
	const string street_name= string("'") + argv[3] + string("'");
	const string post_code 	= string("'") + argv[4] + string("'");
	const string parish 	= string("'") + argv[5] + string("'");
	const string county 	= string("'") + argv[6] + string("'");
	const string district 	= string("'") + argv[7] + string("'");
	double latitude 	= atof(argv[8]);
	double longitude 	= atof(argv[9]);

	DEBUG_MSG("convert ok");	
	// start transaction
	// mysql_query(thisPtr->db, "START TRANSACTION");

	if(!thisPtr->db)
	{
		DEBUG_MSG("db is null");
		return -1;
	}

	if(mysql_query(thisPtr->db, "START TRANSACTION") != 0)
	{
		DEBUG_MSG("[RCApplication::addCb] Start transaction: " << mysql_error(thisPtr->db));
		return -1;
	}

	DEBUG_MSG("start TRANSACTION");	
	try
	{
		DEBUG_MSG("in try()");	
		addRegion(post_code, operator_id, parish, county, district);
		addLocation(latitude, longitude, post_code, street_name);
		
		// retrieve location ID which is generated by an AUTO_INCREMENT;
		int locationID = mysql_insert_id(thisPtr->db);

		addLamppost(locationID, address);
		// addParkingSpace(locationID);
	}
	catch(invalid_argument& e)
	{
		DEBUG_MSG(e.what());
		// rollback transaction
		mysql_query(thisPtr->db, "ROLLBACK");
		return -1;
	}

	// else, no error
	mysql_query(thisPtr->db, "COMMIT");
	DEBUG_MSG("[RCGateway::addCb] Commit in database");

	return 0;
}

void RCApplication::addRegion(string post_code, int operator_id, string parish, string county, string district)
{
	stringstream query;
	
	DEBUG_MSG("ok");
	// check if region already exists
	query << "SELECT post_code FROM region WHERE post_code=" << post_code;
	
	DEBUG_MSG("ok2");
	// execute query
	DEBUG_MSG("[RCGateway::addCb] " << query.str());
	if(mysql_query(thisPtr->db, query.str().c_str()) != 0)
	{
		char str[256];
		snprintf(str, sizeof(str), "[RCGateway::addCb] Invalid post_code(%s)" ,post_code.c_str());
		throw invalid_argument(str);
	}

	// clear query
	query.str("");

    // get the result set
    MYSQL_RES *res = mysql_store_result(thisPtr->db);
    // get the number of the columns
    int num_fields = mysql_num_fields(res);
    // get rows content
    MYSQL_ROW row = mysql_fetch_row(res);

	if(res != NULL)
		mysql_free_result(res);

   	if((num_fields == 1) && (row))
   	{
       	DEBUG_MSG("[RCApplication::addCb] Region with post_code(" << row[0] << ") exists");
       	return;
   	}

   	// else, query returned an empty set.
	// INSERT new region: post_code, operatorID, parish, county, district
	query << "INSERT INTO region VALUES(";
	query << post_code << ",";
	query << operator_id << ",";
	query << parish << ",";
	query << county << ",";
	query << district << ")";

	// execute query
	DEBUG_MSG("[RCGateway::addCb] " << query.str());
	if(mysql_query(thisPtr->db, query.str().c_str()) != 0)
	{
		char str[256];
		snprintf(str, sizeof(str), "[RCGateway::addCb] Invalid operator_id(%d) or (%s)" ,operator_id, post_code.c_str());
		throw invalid_argument(str);
	}
}

// INSERT location: id, latitude, longitude, post_code, street_name
void RCApplication::addLocation(double latitude, double longitude, string post_code, string street_name)
{
	stringstream query;
	query << "INSERT INTO location VALUES(";
	query << "NULL" << ","; // autoincrement location ID
	query << latitude << ",";
	query << longitude << ",";
	query << post_code << ",";
	query << street_name << ")";

	// execute query
	DEBUG_MSG("[RCGateway::addCb] " << query.str());
	if(mysql_query(thisPtr->db, query.str().c_str()) != 0)
	{
		char str[256];
		snprintf(str, sizeof(str), "[RCGateway::addCb] Insert location: latitude(%f) longitude(%f) exists" ,latitude, longitude);
		throw invalid_argument(str);
	}
}

// INSERT lamppost: locationID, address, status
void RCApplication::addLamppost(int lamppost_id, int address)
{
	stringstream query;
	query << "INSERT INTO lamppost VALUES(";
	query << lamppost_id << ",";
	query << address << ",";
	query << "'OFF'" << ")";

	// execute query
	DEBUG_MSG("[RCGateway::addCb] " << query.str());
	if(mysql_query(thisPtr->db, query.str().c_str()) != 0)
	{
		char str[256];
		snprintf(str, sizeof(str), "[RCGateway::addCb] Invalid lamppost_id(%d) or address(%d) exists" ,lamppost_id, address);
		throw invalid_argument(str);
	}
}

// INSERT parking_space: lamppostID, num_vacants
void RCApplication::addParkingSpace(int lamppost_id)
{
	stringstream query;
	query << "INSERT INTO parking_space VALUES(";
	query << lamppost_id << ",";
	query << "0" << ")"; // num vacants default = 0

	// execute query
	DEBUG_MSG("[RCGateway::addCb] " << query.str());
	if(mysql_query(thisPtr->db, query.str().c_str()) != 0)
	{
		char str[256];
		snprintf(str, sizeof(str), "[RCGateway::addCb] Insert parking_space: %s", mysql_error(thisPtr->db));
		throw invalid_argument(str);
	}
}
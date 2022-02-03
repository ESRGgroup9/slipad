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
		thisPtr->tcp.push("MOD;FAIL");
		return -1;
	}

	thisPtr->tcp.push("MOD;OK");
	return 0;
}

int RCApplication::consultCb(int argc, char *argv[])
{
	if(argc != 2)
	{
		DEBUG_MSG("[RCApplication::consultCb] Usage: CONSULT;<operator_id>");
		return -1;
	}
	
	stringstream query;
	int operator_id = atoi(argv[1]);

	query << "SELECT l.street_name, p.id, p.address, p.status ";
	query << "FROM lamppost p, location l ";
	query << "WHERE p.id=l.id AND l.id IN ";

	query << "(SELECT id FROM location WHERE post_code IN ";
	query << "(SELECT post_code FROM region WHERE operator_id="<< operator_id << "))";

	// execute query
	DEBUG_MSG("[RCApplication::consultCb] " << query.str());
	if(mysql_query(thisPtr->db, query.str().c_str()) != 0)
	{
		DEBUG_MSG("[RCApplication::consultCb] Invalid consult: " << mysql_error(thisPtr->db));
		thisPtr->tcp.push("CONSULT;FAIL");
		return -1;
	}

	string street_name;
	int lamppost_id;
	int address;
	string status;

    // get the result set
    MYSQL_RES *res = mysql_store_result(thisPtr->db);
    MYSQL_ROW row;

    // get the number of the columns
	int num_fields = mysql_num_fields(res);
    int err = 0;
    int num_lamppost = 0;

    if(num_fields != 4)
    {
    	DEBUG_MSG("[RCApplication::consultCb] Invalid result with " << num_fields << " columns instead of 4");
    	thisPtr->tcp.push("CONSULT;FAIL");
  		err = -1;
    }
    else
    {
    	while((row = mysql_fetch_row(res)))
	    {
	    	street_name = row[0];
	    	lamppost_id = atoi(row[1]);
	    	address 	= atoi(row[2]);
	    	status 		= row[3];

	    	char str[256];
	    	// snprintf(str, sizeof(str), "\nSTREET : %s\nID     : %d\nADDR   : %d\nSTATUS : %s",
	    	// 	street_name.c_str(), lamppost_id, address, status.c_str());
	    	snprintf(str, sizeof(str), "%s;%d;%d;%s",street_name.c_str(), lamppost_id, address, status.c_str());

	    	DEBUG_MSG("[RCApplication::consultCb] " << string(str));
			
			// send lamppost info to the remote client
	    	thisPtr->tcp.push(str);

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
	if(argc != 3)
	{
		DEBUG_MSG("[RCApplication::signInCb] Usage: SIGNIN;<operator_id>;<password>");
		return -1;
	}
	
	stringstream query;
	int operator_id = atoi(argv[1]);
	query << "SELECT id FROM operator WHERE ";
	query << "id=" << operator_id << " AND ";
	query << "password='" << argv[2] << "'";

	// execute query
	// DEBUG_MSG("[RCApplication::signInCb] " << query.str());
	if(mysql_query(thisPtr->db, query.str().c_str()) != 0)
	{
		DEBUG_MSG("[RCApplication::signInCb] Invalid sign IN: " << mysql_error(thisPtr->db));
		thisPtr->tcp.push("SIGNIN;FAIL");
		return -1;
	}

	// just check if the query returned an empty set or not
	// get the result set
	MYSQL_RES *res = mysql_store_result(thisPtr->db);
    MYSQL_ROW row = mysql_fetch_row(res);
    // get the number of the columns
    int num_fields = mysql_num_fields(res);

   	if(res != NULL)
    	mysql_free_result(res);

   	if((num_fields == 1) && (row))
   	{
		// operator_id is the same as atoi(argv[1]), as declared before
        operator_id = atoi(row[0]);
        DEBUG_MSG("[RCApplication::signInCb] Operator with id(" << operator_id << ") signed IN");
   	}
   	else
   	{
   		DEBUG_MSG("[RCApplication::signInCb] Invalid sign IN: operator("<< operator_id << ") credentials doesnt match");
   		thisPtr->tcp.push("SIGNIN;FAIL");
   		return -1;
   	}

   	// send sign in confirmation
   	thisPtr->tcp.push("SIGNIN;OK");

	return 0;
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
	// DEBUG_MSG("[RCApplication::signUpCb] " << query.str());
	if(mysql_query(thisPtr->db, query.str().c_str()) != 0)
	{
		DEBUG_MSG("[RCApplication::signUpCb] Insert signUp: " << mysql_error(thisPtr->db));
		thisPtr->tcp.push("SIGNUP;FAIL");
		return -1;
	}

	// retrieve operator ID
	int operator_id = mysql_insert_id(thisPtr->db);
	DEBUG_MSG("[RCApplication::signUpCb] New operator '"<< argv[1] << "' has id(" << operator_id << ")");
	
	// confirm sign up
	thisPtr->tcp.push("SIGNUP;OK");

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
	
	// start transaction
	mysql_query(thisPtr->db, "START TRANSACTION");

	try
	{
		addRegion(post_code, operator_id, parish, county, district);
		addLocation(latitude, longitude, post_code, street_name);
		
		// retrieve location ID which is generated by an AUTO_INCREMENT;
		int lamppostID = mysql_insert_id(thisPtr->db);

		addLamppost(lamppostID, address);
	}
	catch(invalid_argument& e)
	{
		DEBUG_MSG(e.what());
		// DEBUG_MSG("Query error:  " << mysql_error(thisPtr->db));

		thisPtr->tcp.push("ADD;FAIL");

		// rollback transaction
		mysql_query(thisPtr->db, "ROLLBACK");
		return -1;
	}

	// else, no error
	mysql_query(thisPtr->db, "COMMIT");
	// DEBUG_MSG("[RCGateway::addCb] Commit in database");
	thisPtr->tcp.push("ADD;OK");
	return 0;
}

void RCApplication::addRegion(string post_code, int operator_id, string parish, string county, string district)
{
	stringstream query;

	// check if region already exists
	query << "SELECT post_code FROM region WHERE post_code=" << post_code;

	// execute query
	// DEBUG_MSG("[RCGateway::addCb] " << query.str());
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
	// DEBUG_MSG("[RCGateway::addCb] " << query.str());
	if(mysql_query(thisPtr->db, query.str().c_str()) != 0)
	{
		char str[256];
		snprintf(str, sizeof(str), "[RCGateway::addCb] Invalid operator_id(%d) or (%s)" ,operator_id, post_code.c_str());
		throw invalid_argument(str);
	}

	DEBUG_MSG("[RCGateway::addCb] Adding region " << parish << " in " << post_code << " - " << county << ", " << district);
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
	// DEBUG_MSG("[RCGateway::addCb] " << query.str());
	if(mysql_query(thisPtr->db, query.str().c_str()) != 0)
	{
		char str[256];
		snprintf(str, sizeof(str), "[RCGateway::addCb] Insert location: latitude(%f) longitude(%f) exists" ,latitude, longitude);
		throw invalid_argument(str);
	}

	DEBUG_MSG("[RCGateway::addCb] Adding location " << street_name << " in " << post_code << " [lat:" << latitude << "; long:" << longitude << "]");
}

// INSERT lamppost: locationID, address, status
void RCApplication::addLamppost(int lamppost_id, int address)
{
	stringstream query;
	query << "INSERT INTO lamppost(id, address) VALUES(";
	query << lamppost_id << ",";
	query << address << ")";

	// execute query
	// DEBUG_MSG("[RCGateway::addCb] " << query.str());
	if(mysql_query(thisPtr->db, query.str().c_str()) != 0)
	{
		char str[256];
		snprintf(str, sizeof(str), "[RCGateway::addCb] Invalid lamppost_id(%d) or address(%d) exists" ,lamppost_id, address);
		throw invalid_argument(str);
	}

	DEBUG_MSG("[RCGateway::addCb] Adding lamppost(" << lamppost_id << ") with address(" << address << ")");
}

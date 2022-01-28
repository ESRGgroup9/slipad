#include "RCApplication.h"
#include "debug.h"
#include <sstream>
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

int RCApplication::addCb(int argc, char *argv[])
{
	/*
	This command must perform:
	- (MAY) Add a new region - post_code, operatorID, parish, county, district
	- Add a new location - (ID) latitude, longitude, post_code, street_name
	- Add a new lamppost - (locationID) address, status
	- Create a parking_space (locationID)
	*/
	if(argc != 10)
	{
		DEBUG_MSG("[RCApplication::addCb] Usage: ADD;<operator_id>;<addr><street_name> <post_code> <parish> <county> <district> <latitude> <longitude>");
		return -1; 
	}

	// improve readability
	int operator_id = atoi(argv[1]);
	int address = atoi(argv[2]);
	const string street_name= string("'") + argv[3] + string("'");
	const string post_code 	= string("'") + argv[4] + string("'");
	const string parish 	= string("'") + argv[5] + string("'");
	const string county 	= string("'") + argv[6] + string("'");
	const string district 	= string("'") + argv[7] + string("'");
	double latitude = atof(argv[8]);
	double longitude = atof(argv[9]);
	// string query
	stringstream query;

	// check if region already exists
	// ...

	// start transaction
	mysql_query(thisPtr->db, "START TRANSACTION");

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
		mysql_query(thisPtr->db, "ROLLBACK");
		DEBUG_MSG("[RCGateway::addCb] Invalid operator_id(" << operator_id << ") or post_code(" << post_code << ")");	
		return -1;
	}

	// clear query
	query.str("");
	// INSERT location: id, latitude, longitude, post_code, street_name
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
		mysql_query(thisPtr->db, "ROLLBACK");
		DEBUG_MSG("[RCGateway::addCb] Insert location: latitude("<< latitude << "), longitude(" << longitude << ") exists");	
		return -1;
	}

	// retrieve location ID which is generated by an AUTO_INCREMENT;
	int locationID = mysql_insert_id(thisPtr->db);
	string status = "'OFF'";
	// clear query
	query.str("");
	// INSERT lamppost: locationID, address, status
	query << "INSERT INTO lamppost VALUES(";
	query << locationID << ",";
	query << address << ",";
	query << status << ")";

	// execute query
	DEBUG_MSG("[RCGateway::addCb] " << query.str());
	if(mysql_query(thisPtr->db, query.str().c_str()) != 0)
	{
		mysql_query(thisPtr->db, "ROLLBACK");
		DEBUG_MSG("[RCGateway::addCb] Invalid locationID(" << locationID << "), address("<< address << ") or status(" << status << ")");	
		return -1;
	}

	// clear query
	query.str("");
	// INSERT parking_space: lamppostID, num_vacants
	query << "INSERT INTO parking_space VALUES(";
	query << locationID << ",";
	query << "0" << ")"; // num vacants default = 0

	// execute query
	DEBUG_MSG("[RCGateway::addCb] " << query.str());
	if(mysql_query(thisPtr->db, query.str().c_str()) != 0)
	{
		mysql_query(thisPtr->db, "ROLLBACK");
		DEBUG_MSG("[RCGateway::addCb] Insert parking_space: " << mysql_error(thisPtr->db));	
		return -1;
	}

	mysql_query(thisPtr->db, "COMMIT");
	DEBUG_MSG("[RCGateway::addCb] Commit in database");

	return 0;
}

int RCApplication::modifyCb(int argc, char *argv[])
{
	if(argc != 2)
	{
		// DEBUG_MSG("[RCApplication::modifyCb] Usage: MOD <operator_id> <lamppost_id>");
		DEBUG_MSG("[RCApplication::modifyCb] Usage: MOD <lamppost_id>");
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
		DEBUG_MSG("[RCApplication::consultCb] Usage: CONSULT <post_code>");
		return -1;
	}
	
	// stringstream query;
	// int operator_id = atoi(argv[1]);
	// query << "SELECT * FROM lamppost WHERE ";
	// // query << "id=" << operator_id << " AND ";
	// // query << "name='" << argv[2] << "' AND ";
	// // query << "password='" << argv[3] << "'";

	// // execute query
	// DEBUG_MSG("[RCApplication::signInCb] " << query.str());
	// if(mysql_query(thisPtr->db, query.str().c_str()) != 0)
	// {
	// 	DEBUG_MSG("[RCApplication::signInCb] Invalid sign IN: " << mysql_error(thisPtr->db));
	// 	return -1;
	// }

	// // just check if the query returned an empty set or not
	// MYSQL_RES *res;
 //    MYSQL_ROW row;
 //    int err = 0;
 //    int num_fields = 0;

 //    // get the result set
 //    res = mysql_store_result(thisPtr->db);

 //    // get the number of the columns
 //    num_fields = mysql_num_fields(res);

  //  	if((num_fields == 1) && (row = mysql_fetch_row(res)))
  //  	{
		// if(row[0] != NULL)
		// {
  //       	// operator_id is the same as atoi(argv[1]), as declared before
  //       	operator_id = atoi(row[0]);
  //       	DEBUG_MSG("[RCApplication::signInCb] Operator with id(" << operator_id << ") signed IN");
		// }
  //  	}
  //  	else
  //  	{
  //  		DEBUG_MSG("[RCApplication::signInCb] Invalid sign IN: operator("<< operator_id << ") credentials doesnt match");
  //  		err = -1;
  //  	}

   	// if(res != NULL)
    // 	mysql_free_result(res);

	// return err;
	return 0;
}

int RCApplication::signInCb(int argc, char *argv[])
{
	if(argc != 4)
	{
		DEBUG_MSG("[RCApplication::signInCb] Usage: SIGNIN <operator_id> <name> <password>");
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
		DEBUG_MSG("[RCApplication::signUpCb] Usage: SIGNUP <name> <password>");
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

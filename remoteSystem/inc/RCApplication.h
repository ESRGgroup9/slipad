/**
 * @file RCApplication.h
 * @author Tomas Abreu, Diogo Fernandes
 * @date 1 fev 2022
 *
 * @brief Implements an Application remote client, from CRemoteClient
 * 
 * This class is used in CRemoteSystem as a result of a new CTCPserver connection.
 */
#ifndef __RCAPPLICATION_H__
#define __RCAPPLICATION_H__

#include "CRemoteClient.h"
#include <mysql/mysql.h>
#include <string>

using std::string;

class RCApplication : public CRemoteClient
{
public:
/**
 * @brief Creates a Remote Client, initializes the command list and the client type
 * @param sd - socket file descriptor used by TCP communication
 * @param database - pointer to the database
 * @return none
 */
	RCApplication(int sd, MYSQL* database);
	~RCApplication();
	
private:
// Callbacks for Application command list
/**
  * @brief  Add new lamppost and its associated region, location and parking_space
  * @param int - number of command arguments
  * @param char** - command arguments
  * @return 0 if successfull, -1 if an error occured
  *
  * Uses addRegion, addLocation and addLamppost. The parking space is inserted
  * when an insert in lamppost table occurs, due to the existance of a trigger
  * for that effect.
  * 
  * Usage: ADD;<operator_id>;<addr>;<street_name>;<post_code>;<parish>;<county>;
  * <district>;<latitude>;<longitude>
  */
	static int addCb(int, char *[]);
  
  // methods used by addCb
  static void addLamppost(int lamppost_id, int address);
  static void addLocation(double latitude, double longitude, string post_code, string street_name);
  static void addRegion(string post_code, int operator_id, string parish, string county, string district);

/**
  * @brief  Modify lamppost
  * @param int - number of command arguments
  * @param char** - command arguments
  * @return 0 if successfull, -1 if an error occured
  *
  * Usage: MOD;<lamppost_id>
  */
	static int modifyCb(int, char *[]);

/**
 * @brief Consult Lamppost Network
 * @param int - number of command arguments
 * @param char** - command arguments
 * @return 0 if successfull, -1 if an error occured
 *
 * Usage: CONSULT;<operator_id>
 */
	static int consultCb(int, char *[]);

/**
 * @brief Check sign in operator credentials
 * @param int - number of command arguments
 * @param char** - command arguments
 * @return 0 if successfull, -1 if an error occured
 *
 * Usage: SIGNIN;<operator_id>;<name>;<password>
 */
	static int signInCb(int, char *[]);

/**
 * @brief Sign up new operator
 * @param int - number of command arguments
 * @param char** - command arguments
 * @return 0 if successfull, -1 if an error occured
 *
 * Usage: SIGNUP;<name>;<password>
 */
	static int signUpCb(int, char *[]);

private:
	static RCApplication* thisPtr;
};

#endif // !__RCAPPLICATION_H__

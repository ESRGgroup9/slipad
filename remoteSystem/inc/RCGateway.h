#ifndef __RCGATEWAY_H__
#define __RCGATEWAY_H__

#include "CRemoteClient.h"
#include <mysql/mysql.h>

class RCGateway : public CRemoteClient
{
public:
/**
 * @brief Creates a Remote Client, initializes the command list and the client type
 * @param sd - socket file descriptor used by TCP communication
 * @param database - pointer to the database
 * @return none
 */
	RCGateway(int sd, MYSQL* database);

private:
// Callbacks for Gateway command list

/**
 * @brief Updates the given lamppost status on the database
 * @param int - number of command arguments
 * @param char** - command arguments
 * @return 0 if successfull, -1 if an error occured
 *
 * Executed when the command (LAMP) from the LS is received.
 * Usage: LAMP <status> <lamppost_id>
 * 
 * - LAMP - command
 * - <status> - lamp status. Can be: MIN | OFF | ON | FAIL
 * - <lamppost_id> - lamppost ID
 * 
 * This must update the lamppost status in the database, to the received status.
 */
	static int lampCb(int, char *[]);
/**
 * @brief Updates the number of available parking spots detected by a given 
 * lamppost on the database
 * @param int - number of command arguments
 * @param char** - command arguments
 * @return 0 if successfull, -1 if an error occured
 *
 * Executed when the command (PARK) from the LS is received.
 * Usage: PARK <num_vacants> <lamppost_id>
 * 
 * - PARK - command
 * - <num_vacants> - number of vacants
 * - <lamppost_id> - lamppost ID
 * 
 * This must update the parking_space vacants number in the database to the 
 * received vacants number.
 */
	static int parkCb(int, char *[]);

/**
 * @brief 
 * @param int - number of command arguments
 * @param char** - command arguments
 * @return 0 if successfull, -1 if an error occured
 *
 * 
 */
	static int crqCb(int, char *[]);

/**
 * @brief 
 * @param int - number of command arguments
 * @param char** - command arguments
 * @return 0 if successfull, -1 if an error occured
 *
 * 
 */
	static int idCb(int, char *[]);

private:
	static RCGateway* thisPtr;
};

#endif // !__RCGATEWAY_H__

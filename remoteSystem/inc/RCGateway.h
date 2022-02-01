/**
 * @file RCGateway.h
 * @author Tomas Abreu, Diogo Fernandes
 * @date 1 fev 2022
 *
 * @brief Implements a gateway remote client, from CRemoteClient
 * 
 * This class is used in CRemoteSystem as a result of a new CTCPserver connection.
 */
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
    ~RCGateway();
    
private:
// Callbacks for Gateway command list

/**
 * @brief Updates the given lamppost status on the database
 * @param int - number of command arguments
 * @param char** - command arguments
 * @return 0 if successfull, -1 if an error occured
 *
 * Usage: LAMP;<status>;<lamppost_id>
 * 
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
 * Usage: PARK;<num_vacants>;<lamppost_id>
 * 
 * - <num_vacants> - number of vacants
 * - <lamppost_id> - lamppost ID
 * 
 * This must update the parking_space vacants number in the database to the 
 * received vacants number.
 */
	static int parkCb(int, char *[]);

/**
 * @brief Define gateway sockfd for the lamppost that sent this command and
 * return its ID.
 * @param int - number of command arguments
 * @param char** - command arguments
 * @return 0 if successfull, -1 if an error occured
 *
 * Usage: CRQ;<id>;<lamppost_addr>
 * 
 * - <id> - lamppost id
 * - <lamppost_addr> - lamppost address
 * 
 * This must update the lamppost sockfd in the database, to this gateway sockfd,
 * and send to the LS its ID.
 */
    static int crqCb(int, char *[]);

private:
    static void dynamicLightUp(int lamppostId);

private:
	static RCGateway* thisPtr;
};

#endif // !__RCGATEWAY_H__

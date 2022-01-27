#ifndef __RCAPPLICATION_H__
#define __RCAPPLICATION_H__

#include "CRemoteClient.h"
#include <mysql/mysql.h>

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

private:
	// Callbacks for Application command list
	// Add new lamppost
	// Modify lamppost
	// Consult Lamppost Network
	static int addCb(int, char *[]);
	static int modifyCb(int, char *[]);
	static int consultCb(int, char *[]);

private:
	static RCApplication* thisPtr;
};

#endif // !__RCAPPLICATION_H__

/**
 * @file RCWebsite.h
 * @author Tomas Abreu, Diogo Fernandes
 * @date 1 fev 2022
 *
 * @brief Implements a website remote client, from CRemoteClient
 * 
 * This class is used in CRemoteSystem as a result of a new CTCPserver connection.
 */
#ifndef __RCWEBSITE_H__
#define __RCWEBSITE_H__

#include "CRemoteClient.h"
#include <mysql/mysql.h>

class RCWebsite : public CRemoteClient
{
public:
	RCWebsite(int sd, MYSQL* database);
	~RCWebsite();
	
protected:

private:
	static RCWebsite* thisPtr;
};

#endif // !__RCWEBSITE_H__

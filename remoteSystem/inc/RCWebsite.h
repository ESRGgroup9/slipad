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

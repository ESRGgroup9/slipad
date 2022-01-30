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
	static void addParkingSpace(int lamppost_id);
	static void addLamppost(int lamppost_id, int address);
	static void addLocation(double latitude, double longitude, string post_code, string street_name);
	static void addRegion(string post_code, int operator_id, string parish, string county, string district);
// Callbacks for Application command list
/**
  * @brief  Add new lamppost and its associated region, location and parking_space
  * @param 
  * @return 
  *
  * Detailed description
  */
	static int addCb(int, char *[]);
	
/**
  * @brief  Modify lamppost
  * @param 
  * @return 
  *
  * Detailed description
  */
	static int modifyCb(int, char *[]);

/**
 * @brief Consult Lamppost Network
 * @param 
 * @return 
 *
 * Detailed description
 */
	static int consultCb(int, char *[]);

/**
 * @brief Check sign in operator credentials
 * @param 
 * @return 
 *
 * Detailed description
 */
	static int signInCb(int, char *[]);

/**
 * @brief Sign up new operator
 * @param 
 * @return 
 *
 * Detailed description
 */
	static int signUpCb(int, char *[]);

protected:
	static RCApplication* thisPtr;
};

#endif // !__RCAPPLICATION_H__

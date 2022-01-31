/**
 * @file CTCPcomm.h
 * @author Tomas Abreu, Diogo Fernandes
 * @date 25 jan 2022
 *
 * @brief Class that implements a TCP communication derived by CCommunication
 * class
 */
#ifndef __CTCPCOMM_H__
#define __CTCPCOMM_H__

#include "CCommunication.h"
#include <string>

class CTCPComm : public CCommunication
{
public:
/**
 * @brief Creates a CTCPcomm object initializing the socket file descriptor as
 * non valid. 
 * @return none
 *
 * In order to use TCP communication, one must call 'setSockfd' before the use
 * of 'send' and 'recv' functions (inherited from CCommunication).
 */
	CTCPComm(int sd = -1);

/**
 * @brief Sets the socket file descriptor to be used in TCP communications
 * @param sd - socket file descriptor
 * @return none
 */
	void setSockfd(int sd);

protected:
/**
 * @brief Receives a message using TCP
 * @param msg - received string
 * @return int - non zero if successfull. -1 if an error occurred
 */
	virtual int recvFunc(std::string &msg);
/**
 * @brief Sends a message using TCP
 * @param msg - string to be sent
 * @return int - non zero if successfull. -1 if an error occurred
 */
	virtual int sendFunc(std::string msg);

private:
	int sockfd;
};

#endif // !__CTCPCOMM_H__

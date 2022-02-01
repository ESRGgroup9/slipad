/**
 * @file CLoraComm.h
 * @author Tomas Abreu, Diogo Fernandes
 * @date 23 jan 2022
 *
 * @brief Defines class to use LoRaClass functions, inheriting from
 * CCommunication
 */
#ifndef __CLORACOMM_H__
#define __CLORACOMM_H__

#include "CCommunication.h"
#include "LoRa.h"
#include <string>

class CLoraComm : public CCommunication
{
public:
/**
 * @brief Create a CLoraComm object
 * @param freqMhz - operation frequency in MHz
 * @param dest - destination address
 * @param std - local address
 * @return none
 *
 * Setups lora communication, setting all necessary variables
 */
	CLoraComm(int freqMhz, int dest, int src);
	~CLoraComm();

/**
 * @brief get local address
 * @return int - local address
 */
	int getLocalAddr(void) const;

/**
 * @brief Updates local address
 * @param int - local address
 * @return none
 */
    void setSrcAddr(int src);

/**
 * @brief Updates destination address
 * @param int - destination address
 * @return none
 */
    void setDestAddr(int dest);

/**
 * @brief Returns last generated LoRaMsg
 * @return LoRaMsg
 */
    LoRaMsg getMsgAttr(void) const;

protected:
/**
 * @brief Receive a message using LoRa communication
 * @param &msg - received message
 * @return int - error code. 0 if message was received, -1 if not
 *
 * Non blocking function for receiveing a message using LoRa communication
 * provided by LoRaClass.
 */
	virtual int recvFunc(std::string &msg);

/**
 * @brief Send a message using LoRa communication
 * @param msg - message to be sent
 * @return int - error code. 0 if message was sent correctly, -1 if not
 *
 * Non blocking function for sending a message using LoRa communication
 * provided by LoRaClass.
 */
	virtual int sendFunc(std::string msg);

private:
	LoRaClass lora;
    LoRaMsg loraMsg;

	int freqMhz;
	int dest_addr;
	int local_addr;
};
// End  CLoraComm class definition

#endif // !__CLORACOMM_H__

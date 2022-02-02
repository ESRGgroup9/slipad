/**
 * @file CLoraComm.h
 * @author Tomas Abreu, Diogo Fernandes
 * @date 23 jan 2022
 *
 * @brief Defines class to use LoRaClass functions, inheriting from
 * CCommunication
 */
#include "CLoraComm.h"
#include "utils.h"
#include "debug.h"
#include <bcm2835.h>
using namespace std;

// default LoRa pins
#define LORA_SS_PIN        RPI_V2_GPIO_P1_11
#define LORA_RESET_PIN     RPI_GPIO_P1_22
#define LORA_DIO0_PIN      RPI_GPIO_P1_18

CLoraComm::CLoraComm(int freqMhz, int dest, int src)
{
	this->freqMhz = freqMhz;
	
	// set LoRa local address
	setSrcAddr(src);

	// set default destination address
	setDestAddr(dest);
	
	// set LoRa pins
	lora.setPins(LORA_SS_PIN, LORA_RESET_PIN, LORA_DIO0_PIN);
	
	// set frequency
	// if(!lora.begin(freqMhz*1E6))
		// panic("CLoraComm::CLoraComm(): lora begin()");
 
	status = ConnStatus::ONLINE;
}

CLoraComm::~CLoraComm()
{

}

int CLoraComm::recvFunc(string &msg)
{
	LoRaError err;

	err = lora.receive(loraMsg);
	if(LoRaError::MSGOK == err)
		msg = loraMsg.msg;
	
	return static_cast<int>(err);
}

int CLoraComm::sendFunc(std::string msg)
{
	loraMsg = lora.sendTo(msg, dest_addr);
	return 0;
}

void CLoraComm::setSrcAddr(int src)
{
	this->local_addr = src;
	// set LoRa local address
	lora.setLocalAddress(src);
}

void CLoraComm::setDestAddr(int dest)
{
	this->dest_addr = dest;
}

int CLoraComm::getLocalAddr(void) const
{
	return local_addr;
}

LoRaMsg CLoraComm::getMsgAttr(void) const
{
	return loraMsg;
}
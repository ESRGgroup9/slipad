#include "CLoraComm.h"
#include "LoRa.h"
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
	this->dest_addr = dest;
	this->local_addr = src;

	// set LoRa local address
	lora.setLocalAddress(src);

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
	LoRaMsg loraMsg;
	LoRaError err;

	err = lora.receive(loraMsg);
	if(LoRaError::MSGOK == err)
		msg = loraMsg.msg;

	return static_cast<int>(err);
}

int CLoraComm::sendFunc(std::string msg)
{
	LoRaMsg loraMsg;
	loraMsg = lora.sendTo(msg, dest_addr);
	return 0;
}

int CLoraComm::getLocalAddr(void) const
{
	return local_addr;
}

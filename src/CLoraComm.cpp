#include "CLoraComm.h"
#include "utils.h"
#include "debug.h"
 
using namespace std;

// Create LoRa object
static LoRaClass lora;

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

int CLoraComm::getLocalAddr(void) const
{
	return local_addr;
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
#include "CLoraComm.h"
#include "utils.h"
#include "../lora/LoRa.h"
using namespace std;

uint8_t localAddr = 0xBB;
LoRaClass lora(localAddr);

CLoraComm::CLoraComm(int freqMhz, int dest, int src)
{
	this->freqMhz = freqMhz;
	this->dest_addr = dest;
	this->local_addr = src;//>>>>>>>>>>>>>>>>>>>>> set lora local Addr with this

	// set Lora pins
	lora.setPins(LORA_SS_PIN, LORA_RESET_PIN, LORA_DIO0_PIN);
	
	// set frequency
	if(!lora.begin(freqMhz*1E6))
		panic("CLoraComm::CLoraComm(): lora begin()");

	status = ConnStatus::ONLINE;
	// init(25);
}

CLoraComm::~CLoraComm()
{

}

int CLoraComm::getLocalAddr(void) const
{
	return local_addr;
}

string CLoraComm::recvFunc(void)
{
	LoRaMsg loraMsg;
	LoRaError err;

	err = lora.receive(loraMsg);
	
	return loraMsg.msg;
}

int CLoraComm::sendFunc(std::string msg)
{
	LoRaMsg loraMsg;
	loraMsg = lora.sendTo(msg, dest_addr);
	return 0;
}
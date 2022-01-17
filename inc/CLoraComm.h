#ifndef __CLORACOMM_H__
#define __CLORACOMM_H__

#include <string>
#include "CCommunication.h"
#include "LoRa.h" // LoRaError

#include <bcm2835.h>

// default LoRa pins
#define LORA_SS_PIN        RPI_V2_GPIO_P1_11
#define LORA_RESET_PIN     RPI_GPIO_P1_22
#define LORA_DIO0_PIN      RPI_GPIO_P1_18

class CLoraComm : public CCommunication
{
public:
	CLoraComm(int freqMhz, int dest, int src);
	~CLoraComm();

	int getLocalAddr(void) const;

protected:
	int recvFunc(std::string &msg);
	int sendFunc(std::string msg);

private:
	int freqMhz;
	int dest_addr;
	int local_addr;
};
// End  CLoraComm class definition

#endif // !__CLORACOMM_H__

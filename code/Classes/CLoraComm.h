#ifndef __CLORACOMM_H__
#define __CLORACOMM_H__

class CLoraComm : public LoRaClass
{

	void digitalWrite(uint8_t pin, uint8_t val);
	void pinMode(uint8_t pin);
	void bitWrite();
	void attachInterrupt();
	void detachInterrupt();
	uint8_t digitalPinToInterrupt();

	void setSPI(void);

}; // end class CLoraComm definition

#endif // !__CLORACOMM_H__

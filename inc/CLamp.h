#ifndef __CLAMP_H__
#define __CLAMP_H__

#include <pthread.h>

class CLamp
{
public:
	CLamp(unsigned char timoutSecs);
	~CLamp();

	void setBrightness(unsigned char lux);
	const unsigned char getBrightness(void);

	void on(unsigned char lux);
	void off(void);

protected:
	
private:
	pthread_mutex_t mutChangePWM;
	unsigned char pwmVal; // BE CAREFUL uint8_t
	const unsigned char timLapOnSecs;
};
// End CLamp class definition

#endif // !__CLAMP_H__

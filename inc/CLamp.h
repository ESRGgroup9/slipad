#ifndef __CLAMP_H__
#define __CLAMP_H__

#include "timer.h"

#include <pthread.h>
#include <cstdint>

class CLamp
{
public:
	CLamp(int timoutSecs);
	~CLamp();

	void setBrightness(uint8_t lux);
	uint8_t getBrightness(void) const;

	void on(uint8_t lux);
	void off(void);

private:
	pthread_mutex_t mutChangePWM;
	uint8_t pwmVal; // BE CAREFUL uint8_t
	Timer timLampOnSecs;
};
// End CLamp class definition

#endif // !__CLAMP_H__

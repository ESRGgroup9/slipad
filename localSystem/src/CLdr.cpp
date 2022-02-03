#include "CLdr.h"
#include "TSL2581.h"
#include "utils.h"

// defines when limit value for day and night lux
#define DAY_NIGHT_LUX_LIMIT 	60

CLdr::CLdr()
{
	if(!Init_TSL2581())
		panic("Init_TSL2581()");
}
	
CLdr::~CLdr()
{

}

LuxState CLdr::getLuxState(void)
{	
	int lux = calculateLux(2);
	//printf("lux = %d \r\n", lux);

	if(lux < DAY_NIGHT_LUX_LIMIT)
		luxstate = LuxState::NIGHT;
	else
		luxstate = LuxState::DAY;

	return luxstate;
}

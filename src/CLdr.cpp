#include "CLdr.h"
#include "TSL2581.h"
#include "utils.h"
#include <stdlib.h>

// defines when limit value for day and night lux
#define DAY_NIGHT_LUX_LIMIT 	60

CLdr::CLdr()
{
	if(!Init_TSL2581())
		panic("Can not init device TSL2581 !\n");
};
	
CLdr::~CLdr()
{

};

luxState_enum CLdr::getLuxState(void)
{	
	int lux = calculateLux(2);
	//printf("lux = %d \r\n", lux);

	if(lux<DAY_NIGHT_LUX_LIMIT)
		luxstate = NIGHT;
	else 
		luxstate = DAY;

	return luxstate;
}

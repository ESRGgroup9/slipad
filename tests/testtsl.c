#include <stdio.h>      //printf()
#include <stdlib.h>     //exit()
#include <signal.h>

#include "DEV_Config.h"
#include "TSL2581.h"

void  Handler(int signo)
{
    //System Exit
    printf("\r\nHandler:Program stop\r\n"); 
    
    DEV_ModuleExit();

    exit(0);
}

int main(int argc, char **argv)
{
	int  lux;

    //TSL2581 initialization
	Init_TSL2581();

    // Exception handling:ctrl + c
    signal(SIGINT, Handler);
	
	//because The lower four bits are the silicon version number
	//printf("READ ID = 0x%02X\n\t", Read_ID() & 0xf0);
	
	while(1)
	{		
		lux  =  calculateLux(2);
		printf("lux = %d \r\n", lux);
		DEV_Delay_ms(1000);
	}

	DEV_ModuleExit();
    return 0; 
}
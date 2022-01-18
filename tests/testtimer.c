#include <stdio.h>
#include "timer.h"
#include <sys/signal.h> // sigval

void tim1(union sigval arg)
{
	static int i = 0;
	printf("In tim1() for the %d time\n", i++);
}

void tim2(union sigval arg)
{
	static int i = 0;
	printf("In tim2() for the %d time\n", i++);
}

int main(int argc, char *argv[])
{
	// create_timer(5, tim1);
	// create_timer(10, tim2);
	Timer tim1(5, tim1);
	// Timer tim2(10, tim2);

	
	while(1)
		;

	return 0;
}
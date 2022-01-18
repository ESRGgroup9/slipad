#include <stdio.h>
#include <sys/signal.h> // sigval

#include "timer.h"

int j = 0;

void timer1(union sigval arg)
{
	static int i = 1;
	printf("In tim1() for the %d time\n", i++);
	j++;
}

void timer2(union sigval arg)
{
	static int i = 1;
	printf("In tim2() for the %d time\n", i++);
}

int main(int argc, char *argv[])
{
	Timer tim1(2, timer1);
	Timer tim2(4, timer2);

	printf("Wait for 3 tim1() interrupts...\n\n");
	tim1.start();
	tim2.start();

	while(j != 3)
		;

	tim1.stop();
	tim2.stop();

	while(1)
		;

	return 0;
}
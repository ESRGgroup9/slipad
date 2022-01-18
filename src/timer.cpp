#include "timer.h"
#include "utils.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * ORIGINAL HEADER 
 *
 * sigev_thread.c
 *
 * Demonstrate use of the SIGEV_THREAD signal mode to handle
 * signals by creating a new thread.
 *
 * Special notes: This program will not compile on Solaris 2.5.
 * It will compile on Digital UNIX 4.0 but will not work.
 * Digital UNIX 4.0c fixes SIGEV_THREAD, and sources inform me
 * that Solaris 2.6 will also fix SIGEV_THREAD. To try this on
 * Solaris 2.5, remove the "#ifdef sun" conditionals in main.
 *
 * Original: 
 * http://ptgmedia.pearsoncmg.com/images/0201633922/sourcecode/sigev_thread.c 
 */

/**
 * CIRO's Header
 * This is a slightly modified version of the original one so that 
 * i can create multiple timers and trigger then in a more 
 * handy way for my project.
 */ 
Timer::Timer(unsigned seconds, void (*handler)(union sigval arg))
{
	int status;

	/*
	* Set the sigevent structure to cause the signal to be
	* delivered by creating a new thread.
	*/
	se.sigev_notify = SIGEV_THREAD;
	se.sigev_value.sival_ptr = &timer_id;
	// set handler function
	se.sigev_notify_function = handler;
	se.sigev_notify_attributes = NULL;

	status = timer_create(CLOCK_REALTIME, &se, &timer_id);
	if (status == -1)
		panic("Create timer");

	// set timer period in seconds
	this->period_secs = period_secs;

	printf("Timer created");
}

Timer::~Timer()
{

}

void Timer::setPeriod(int period_secs)
{
	// period between now and the first timer interrupt
  	ts.it_value.tv_sec = period_secs;
  	ts.it_value.tv_nsec = 0;
  	// period between successive timer interrupts
  	ts.it_interval.tv_sec = period_secs;
 	ts.it_interval.tv_nsec = 0;

	if (timer_settime(timer_id, 0, &ts, 0) == -1)
		panic("Set timer");
}

void Timer::start()
{
	setPeriod(period_secs);
}

void Timer::stop()
{
	setPeriod(0);
}
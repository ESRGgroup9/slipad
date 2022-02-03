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

#include "timer.h"
#include "utils.h"
#include "debug.h"

int Timer::ID = 0;

/**
 * CIRO's Header
 * This is a slightly modified version of the original one so that 
 * i can create multiple timers and trigger then in a more 
 * handy way for my project.
 */ 
Timer::Timer(unsigned seconds, void (*handler)(union sigval arg), bool is_periodic) {
	this->is_periodic = is_periodic;
	// set timer period in seconds
	this->period_secs = seconds;
	// assign id
	this->id = ID++;

	/*
	* Set the sigevent structure to cause the signal to be
	* delivered by creating a new thread.
	*/
	se.sigev_notify = SIGEV_THREAD;

	se.sigev_value.sival_ptr = &timer_id;
	se.sigev_value.sival_int = this->id;
	
	// set handler function
	se.sigev_notify_function = handler;
	se.sigev_notify_attributes = NULL;

	if (timer_create(CLOCK_REALTIME, &se, &timer_id) == -1)
		panic("Create timer");

#ifdef DEBUG
	char msg[64];
	snprintf(msg, sizeof(msg), "Timer[%d] created with %s timeout of %d seconds",
		id, (is_periodic == true ? "continuous" : "single"), period_secs);

	DEBUG_MSG(msg);
#endif // !DEBUG

}

Timer::~Timer()
{

}

/**
 * @brief Sets timer period
 * @param unsigned period_secs
 * @return none
 *
 * Sets timer to expire at the end of 'period_secs'.
 * 
 * If 'is_periodic' is false then one wants the timer to expire at first
 * timeout. Therefore, 'interval' must be 0.
 * 
 * If 'is_periodic' is true then one wants the timer to reload 'interval'
 * each time the timer expires. Therefore 'interval' must be nonzero
 */
void Timer::setPeriod(unsigned period_secs, bool expireNow)
{
	// period between now and the first timer interrupt
  	ts.it_value.tv_sec = period_secs * (!expireNow);
  	ts.it_value.tv_nsec = 0;
  	// period between successive timer interrupts
  	ts.it_interval.tv_sec = period_secs * (is_periodic);
 	ts.it_interval.tv_nsec = 0;

	if (timer_settime(timer_id, 0, &ts, 0) == -1)
		panic("Set timer");
}

void Timer::start(bool expireNow)
{
	setPeriod(period_secs, expireNow);
	DEBUG_MSG("Timer["<< (int)id << "] started with timeout " << period_secs << " seconds");
}

void Timer::stop()
{
	setPeriod(0);
	DEBUG_MSG("Timer[" << (int)id << "] stopped");
}
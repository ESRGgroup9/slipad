#include "utils.h"

#include <pthread.h>
#include <sys/signal.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

// void my_alarm(int seconds)
// {
// 	struct itimerval itv;

// 	// set handling of SIGALRM
// 	signal(SIGALRM,sig_handler);

// 	// period between successive timer interrupts
// 	itv.it_interval.tv_sec = seconds;
// 	itv.it_interval.tv_usec = 0;

// 	// period between now and the first timer interrupt
// 	itv.it_value.tv_sec = seconds;
// 	itv.it_value.tv_usec = 0;
// 	setitimer(ITIMER_REAL, &itv, NULL);
// }

void handler()
{
	
}

void create_timer(unsigned secs)
{
  timer_t timer_id;
  int status;
  struct itimerspec ts;
  struct sigevent se;

  /*
   * Set the sigevent structure to cause the signal to be
   * delivered by creating a new thread.
   */
  se.sigev_notify = SIGEV_THREAD;
  se.sigev_value.sival_ptr = &timer_id;
  // se.sigev_notify_function = handler;
  se.sigev_notify_attributes = NULL;

  ts.it_value.tv_sec = secs;
  ts.it_value.tv_nsec = 0;
  ts.it_interval.tv_sec = secs;
  ts.it_interval.tv_nsec = 0;

  status = timer_create(CLOCK_REALTIME, &se, &timer_id);
  if (status == -1)
    panic("Create timer");

  // TODO maybe we'll need to have an array of itimerspec
  status = timer_settime(timer_id, 0, &ts, 0);
  if (status == -1)
    panic("Set timer");
}
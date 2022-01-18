#ifndef __TIMER_H__
#define __TIMER_H__

#include <sys/signal.h>
#include <ctime> // timer_t

class Timer
{
public:
	Timer(unsigned seconds, void (*handler)(union sigval));
	~Timer();

	void start();
	void stop();

private:
	void setPeriod(unsigned period_secs);

private:
	timer_t timer_id;
	struct itimerspec ts;
  	struct sigevent se;

  	unsigned period_secs;
};

#endif // !__TIMER_H__

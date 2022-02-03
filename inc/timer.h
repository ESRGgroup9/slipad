#ifndef __TIMER_H__
#define __TIMER_H__

#include <sys/signal.h>
#include <ctime> // timer_t

class Timer
{
public:
	Timer(unsigned seconds, void (*handler)(union sigval), bool is_periodic = true);
	~Timer();

	void start(bool expireNow = false);
	void stop();
	int id;

private:
	void setPeriod(unsigned period_secs, bool expireNow = false);
	static int ID;

private:
	timer_t timer_id;
	struct itimerspec ts;
  	struct sigevent se;

  	unsigned period_secs;
  	bool is_periodic;
};

#endif // !__TIMER_H__

#ifndef __CSENSORS_H__
#define __CSENSORS_H__

#include "timer.h"
#include "CPir.h"
#include "CLdr.h"
#include "CFailureDetector.h"

#include <signal.h> // sigval
#include <pthread.h>
#include <mqueue.h> // mqd_t
#include <string>

class CSensors 
{
public:
	CSensors();
	~CSensors();

	void run();

private:
	static void *tReadLdr(void*);
	void sendCmd(std::string cmd);

private:
	static CSensors *thisPtr;

	static void isrHandler(int n, siginfo_t *info, void *unused);
	static void timHandler(union sigval arg);

	void timReadLdrISR();
	void pirISR();
	void lampfISR();

private:
	CPir pir;
	CLdr ldr;
	CFailureDetector lampf;

	pthread_t tReadLdr_id;
	
	pthread_mutex_t mutReadLdr;
	pthread_cond_t condReadLdr;

	mqd_t msgqSensors;
	pid_t mainPID;
	Timer timReadLdr;
};
// End CSensors class definition

#endif // !__CSENSORS_H__

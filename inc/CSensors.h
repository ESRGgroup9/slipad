#ifndef __CSENSORS_H__
#define __CSENSORS_H__

#include "timer.h"
#include "CPir.h"
#include "CLdr.h"
// #include "CFailureDetector.h"

#include <signal.h>
#include <pthread.h>
#include <mqueue.h> // mqd_t
#include <string>

#define TIM_READ_LDR_SECS	(0)

#define SIG_CMD_SENT (SIGUSR1)
#define MSGQ_NAME "/dsensors"

class CSensors 
{
public:
	CSensors();
	~CSensors();

	void run();

private:
	static void *tReadLdr(void*);
	static void pirISR(int n, siginfo_t *info, void *unused);
	static void lampfISR(void);
	static void timReadLdrHandler(union sigval arg);
	
	void sendCmd(std::string cmd);

private:
	CPir pir;
	CLdr ldr;
	// CFailureDetector lampf;

	pthread_mutex_t mutReadLdr;
	// pthread_cond_t condReadLdr;

	pthread_t tReadLdr_id;
	mqd_t msgqSensors;
	pid_t mainPID;
	Timer timReadLdr;
};
// End CSensors class definition

#endif // !__CSENSORS_H__

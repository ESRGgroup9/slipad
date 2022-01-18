#include "CSensors.h"
#include "utils.h"
#include "debug.h"

using namespace std;

CSensors::CSensors() :
	pir(NULL),
	ldr(),
	timReadLdr(TIM_READ_LDR_SECS, timReadLdrHandler)
{
	if(pthread_cond_init(&condReadLdr, NULL) != 0)
		panic("CSensors::CSensors(): Condition variable init");	

	if(pthread_mutex_init(&mutReadLdr, NULL) != 0)
		panic("CSensors::CSensors(): Mutex init");

	msgqSensors = mq_open(MSGQ_NAME, O_RDWR | O_CREAT | O_NONBLOCK, NULL);
	if(msgqSensors == (mqd_t)-1)
		panic("CSensors::CSensors(): Creating message queue");

	if(pthread_create(&tReadLdr_id, NULL, tReadLdr, this) != 0)
		panic("CSensors::CSensors(): pthread_create");

	// init...
	// CFailureDetector lampf;
	// pid_t mainPID;
}

CSensors::~CSensors()
{

}

void CSensors::timReadLdrHandler(union sigval arg)
{
	DEBUG_MSG("[CSensors::timReadLdrHandler] Signal condReadLdr");
	// pthread_cond_signal(&condReadLdr);
}

// max length of a message queue
#define MAX_MSG_LEN_R     10000

void CSensors::run()
{
	char msg[MAX_MSG_LEN_R];
	int err = 0;

	DEBUG_MSG("[CSensors::run] Waiting for main PID...");
	// receive main PID
	do
	{
		// read message from msgqueue
		if(mq_receive(msgqSensors, msg, MAX_MSG_LEN_R, NULL) == -1)
		{
			// get error from errno
			err = errno;

			// error not expected
			if(err != EAGAIN)
				panic("In mq_receive()");
		}
		// while message queue is empty = wait for PID receival
	} while(err == EAGAIN);

	// mainPID received
	mainPID = static_cast<int>(atoi(msg));
	DEBUG_MSG("[CSensors::run] Received main PID[" << mainPID << "]");

	// start sampling sensors
	timReadLdr.start();
}

void *CSensors::tReadLdr(void *arg)
{
	CSensors *c = reinterpret_cast<CSensors*>(arg);
	LuxState luxState = LuxState::DAY;
	LuxState oldLuxState = LuxState::DAY;
	
	while(c)
	{
		pthread_mutex_lock(&c->mutReadLdr);
		
		DEBUG_MSG("[CSensors::tReadLdr] Waiting for condReadLdr...");
		pthread_cond_wait(&c->condReadLdr, &c->mutReadLdr);
		DEBUG_MSG("[CSensors::tReadLdr] Im awake!");

		luxState = c->ldr.getLuxState();
		pthread_mutex_unlock(&c->mutReadLdr);

		// is there a change in lux State?
		if(luxState != oldLuxState)
		{
			if(luxState == LuxState::NIGHT)
			{
				// c->lampf.enable();
				c->pir.enable();
				c->sendCmd("MIN");
			}
			else
			{
				// c->lampf.disable();
				c->pir.disable();
				c->sendCmd("OFF");
			}

			oldLuxState = luxState;
		}
	}

	DEBUG_MSG("[CSensors::tReadLdr] Exiting thread...");
	return NULL;
}

void CSensors::sendCmd(string cmd)
{
	if(mq_send(msgqSensors, cmd.c_str(), cmd.length(), 1) != 0)
		panic("In mq_send()");

	DEBUG_MSG("[CSensors::sendCmd] sent(" << cmd << ")");
	kill(mainPID, SIG_CMD_SENT);
	DEBUG_MSG("[CSensors::sendCmd] signaled PID[" << static_cast<int>(mainPID) << "]");
}

void CSensors::pirISR(void)
{
	// sendCmd("ON");
}

void CSensors::lampfISR(void)
{
	// sendCmd("FAIL");
}

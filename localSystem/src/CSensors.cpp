#include "CSensors.h"
#include "utils.h"
#include "debug.h"
#include "defs.h"

#include <cstring> // memset
using namespace std;

#define TIM_READ_LDR_SECS	(5)

CSensors* CSensors::thisPtr = NULL;

CSensors::CSensors() :
	pir(isrHandler),
	ldr(),
	lampf(isrHandler),
	timReadLdr(TIM_READ_LDR_SECS, timHandler)
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

	// init ISR handler pointer
	thisPtr = this;
}

CSensors::~CSensors()
{

}

void CSensors::pirISR()
{
	DEBUG_MSG("[pirISR] Motion detected - Sending[ON]...");
	sendCmd("ON");
}

void CSensors::lampfISR()
{
	DEBUG_MSG("[lampfISR] Lamp failure detected - Sending[FAIL]...");
	sendCmd("FAIL");
}

void CSensors::isrHandler(int n, siginfo_t *info, void *unused)
{
	if(!thisPtr)
		panic("CSensors::isrHandler(): thisPtr not defined");

	DEBUG_MSG("[CSensors::isrHandler] handling interrupt[" << n << "]...");

	switch(n)
	{
		case PIR_SIG_NUM: //SIGUSR1:
			thisPtr->pirISR();
			break;

		case LAMPF_SIG_NUM: //SIGUSR2: 
			thisPtr->lampfISR();
			break;

		default:
		{
			ERROR_MSG("[CSensors::isrHandler] unexpected ISR num");
		}
	}
}

void CSensors::timReadLdrISR()
{
	DEBUG_MSG("[CSensors::timReadLdrISR] Signal condReadLdr");
	pthread_cond_signal(&condReadLdr);
}

void CSensors::timHandler(union sigval arg)
{
	if(!thisPtr)
		panic("CLS::timHandler(): thisPtr not defined");

	int id = arg.sival_int;
	DEBUG_MSG("[CLS::timHandler] handling timer[" << id << "] timeout...");

	// cannot do switch statement since tim*.id is not a compile time constant
	if(id == thisPtr->timReadLdr.id)
	{
		thisPtr->timReadLdrISR();
	}
	else
	{
		ERROR_MSG("[CLS::timHandler] unexpected timer event");
	}
}

// max length of a message queue
#define MAX_MSG_LEN_R     10000

void CSensors::run()
{
	char msg[MAX_MSG_LEN_R];
	int err = 0;

	// clear message
	memset(msg, 0, sizeof(msg));

	DEBUG_MSG("[CSensors::run] Waiting for main PID...");
	// receive main PID
	do
	{
		// read message from msgqueue
		err = mq_receive(msgqSensors, msg, MAX_MSG_LEN_R, NULL);
		if(err == -1)
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

#ifdef DEBUG
	//lampf.enable();
	//pir.enable();
#endif // !DEBUG
	
	// start sampling LDR sensor
	timReadLdr.start();

	// wait for thread termination
	pthread_join(tReadLdr_id, NULL);
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
				c->lampf.enable();
				c->pir.enable();
				c->sendCmd("MIN");
			}
			else
			{
				c->lampf.disable();
				c->pir.disable();
				c->sendCmd("OFF");
			}

			oldLuxState = luxState;
		}
	}

	DEBUG_MSG("[CSensors::tReadLdr] Exiting thread...");
	return NULL;
}

{
	oldLampfState = lampfState;
	lampfState = lampfReadPin();

	if(oldLampfState == lampfState)
		// lamp if in FAIL or is WORKING for the past two cycles
		return;

	// old state is different that current state
	if(lampfState == FAIL)
	// lamp FAIL
	{
		pir.disable();
		send("FAIL");
	}
	// lamp was in FAIL and now its WORKING
	else
	{
		// reactivate sensors
		pir.enable();
		sendCmd("MIN")
	}
}
MIN->FAIL->MIN
ON->FAIL->MIN
OFF
FAIL

void CSensors::sendCmd(string cmd)
{
	int err = 0;
	do
	{
		if(mq_send(msgqSensors, cmd.c_str(), cmd.length()+1, 1) == -1)
		{
			err = errno;

			if( err != EAGAIN )
				panic("In mq_send()");
		}
	} while (err == EAGAIN);

	DEBUG_MSG("[CSensors::sendCmd] sent(" << cmd << ")");
	kill(mainPID, SIG_dSENSORS);
	// DEBUG_MSG("[CSensors::sendCmd] signaled PID[" << static_cast<int>(mainPID) << "]");
}

#define DEBUG

#include "CSensors.h"
#include "utils.h"
#include "debug.h"
#include "defs.h"

#include <cstring> // memset
using namespace std;

#define TIM_READ_LDR_SECS	(5)
#define TIM_READ_LAMPF_SECS	(5)

static LuxState oldLuxState = LuxState::DAY;
static char oldLampfState = 0;

CSensors* CSensors::thisPtr = NULL;

CSensors::CSensors() :
	pir(isrHandler),
	ldr(),
	lampf(),
	timReadLdr(TIM_READ_LDR_SECS, timHandler),
	timReadLampF(TIM_READ_LAMPF_SECS, timHandler)
{
	if(pthread_cond_init(&condReadLdr, NULL) != 0)
		panic("CSensors::CSensors(): Condition variable init");	

	if(pthread_cond_init(&condReadLampF, NULL) != 0)
		panic("CSensors::CSensors(): Condition variable init");	

	if(pthread_mutex_init(&mutReadLdr, NULL) != 0)
		panic("CSensors::CSensors(): Mutex init");

	if(pthread_mutex_init(&mutReadLampF, NULL) != 0)
		panic("CSensors::CSensors(): Mutex init");

	msgqSensors = mq_open(MSGQ_NAME, O_RDWR | O_CREAT | O_NONBLOCK, NULL);
	if(msgqSensors == (mqd_t)-1)
		panic("CSensors::CSensors(): Creating message queue");

	if(pthread_create(&tReadLdr_id, NULL, tReadLdr, this) != 0)
		panic("CSensors::CSensors(): pthread_create");

	if(pthread_create(&tReadLampF_id, NULL, tReadLampF, this) != 0)
		panic("CSensors::CSensors(): pthread_create");

	// init ISR handler pointer
	thisPtr = this;
}

CSensors::~CSensors()
{

}

void CSensors::pirISR()
{
	// DEBUG_MSG("[pirISR] Motion detected - Sending[ON]...");
	sendCmd("ON");
}

// void CSensors::lampfISR()
// {
// 	DEBUG_MSG("[lampfISR] Lamp failure detected - Sending[FAIL]...");
// 	sendCmd("FAIL");
// }

void CSensors::isrHandler(int n, siginfo_t *info, void *unused)
{
	if(!thisPtr)
		panic("CSensors::isrHandler(): thisPtr not defined");

	// DEBUG_MSG("[CSensors::isrHandler] handling interrupt[" << n << "]...");

	switch(n)
	{
		case PIR_SIG_NUM: //SIGUSR1:
			thisPtr->pirISR();
			break;

		// case LAMPF_SIG_NUM: //SIGUSR2: 
		// 	thisPtr->lampfISR();
		// 	break;

		default:
		{
			ERROR_MSG("[CSensors::isrHandler] unexpected ISR num");
		}
	}
}

void CSensors::timReadLdrISR()
{
	// DEBUG_MSG("[CSensors::timReadLdrISR] Signal condReadLdr");
	pthread_cond_signal(&condReadLdr);
}

void CSensors::timReadLampFISR()
{
	// DEBUG_MSG("[CSensors::timReadLampFISR] Signal condReadLampF");
	pthread_cond_signal(&condReadLampF);
}

void CSensors::timHandler(union sigval arg)
{
	if(!thisPtr)
		panic("CLS::timHandler(): thisPtr not defined");

	int id = arg.sival_int;
	// DEBUG_MSG("[CLS::timHandler] handling timer[" << id << "] timeout...");

	// cannot do switch statement since tim*.id is not a compile time constant
	if(id == thisPtr->timReadLdr.id)
	{
		thisPtr->timReadLdrISR();
	}
	else if( id == thisPtr->timReadLampF.id )
	{
		thisPtr->timReadLampFISR();
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

	stringstream debug_msg;
	debug_msg << "[CSensors::run] Received main PID[" << mainPID << "]";
	DEBUG_MSG(debug_msg.str().c_str());

#ifdef DEBUG
	//lampf.enable();
	//pir.enable();
#endif // !DEBUG

	lampf.enable();
	
	// start sampling LDR sensor
	timReadLdr.start();
	// start sampling Lamp Failure sensor
	// timReadLampF.start();

		// wait for thread termination
	pthread_join(tReadLampF_id, NULL);

	// wait for thread termination
	pthread_join(tReadLdr_id, NULL);

}

#define LAMP_FAIL 	 ('1')
#define LAMP_WORKING ('0')

void *CSensors::tReadLdr(void *arg)
{
	CSensors *c = reinterpret_cast<CSensors*>(arg);
	LuxState luxState = LuxState::DAY;
	
	while(c)
	{
		pthread_mutex_lock(&c->mutReadLdr);
		
		// DEBUG_MSG("[CSensors::tReadLdr] Waiting for condReadLdr...");
		pthread_cond_wait(&c->condReadLdr, &c->mutReadLdr);
		// DEBUG_MSG("[CSensors::tReadLdr] Im awake!");

		oldLuxState = luxState;
		luxState = c->ldr.getLuxState();
		pthread_mutex_unlock(&c->mutReadLdr);

		// is there a change in lux State?
		if( (luxState != oldLuxState) )
		{
			if(luxState == LuxState::NIGHT) 
			{
				// start timer with expire now flag
				c->timReadLampF.start();
				c->pir.enable();
				c->sendCmd("MIN");
			}
			else if(oldLampfState != LAMP_FAIL)
			{
				// c->lampf.disable();
				c->timReadLampF.stop();
				c->pir.disable();
				c->sendCmd("OFF");
			}
		}
	}

	DEBUG_MSG("[CSensors::tReadLdr] Exiting thread...");
	return NULL;
}

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

	stringstream debug_msg;
	debug_msg << "[CSensors::sendCmd] sent(" << cmd << ")";
	DEBUG_MSG(debug_msg.str().c_str());

	kill(mainPID, SIG_dSENSORS);
	// DEBUG_MSG("[CSensors::sendCmd] signaled PID[" << static_cast<int>(mainPID) << "]");
}

void *CSensors::tReadLampF(void *arg)
{
	CSensors *c = reinterpret_cast<CSensors*>(arg);

	char lampfState = 0;

	while(c)
	{
		pthread_mutex_lock(&c->mutReadLampF);
		
		// DEBUG_MSG("[CSensors::tReadLampF] Waiting for condReadLampF...");
		pthread_cond_wait(&c->condReadLampF, &c->mutReadLampF);
		// DEBUG_MSG("[CSensors::tReadLampF] Im awake!");

		oldLampfState = lampfState;
		lampfState = c->lampf.read();

		pthread_mutex_unlock(&c->mutReadLampF);

	    if(lampfState == -1)
	    	panic("On read lampf dev.\n");

	    if((oldLampfState == lampfState))
	        // lamp if in FAIL or is WORKING for the past two cycles
	        continue;

	    // old state is different that current state
	    if(lampfState == LAMP_FAIL)
	    // lamp FAIL
	    {
	        c->pir.disable();
	        c->sendCmd("FAIL");
	    }
	    // lamp is WORKING
	    else if((oldLuxState == LuxState::NIGHT) && (lampfState == LAMP_WORKING))
	    {
	        // lamp was in FAIL and now its WORKING
	        // reactivate sensors
	        c->pir.enable();
	        c->sendCmd("MIN");
	    }
	}

	DEBUG_MSG("[CSensors::tReadLampF] Exiting thread...");
	c->lampf.disable();
	return NULL;
}

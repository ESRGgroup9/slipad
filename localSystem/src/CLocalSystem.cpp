#define DEBUG

#include "CLocalSystem.h"
#include "utils.h" // panic
#include "debug.h"
#include "defs.h"

#include <unistd.h> // getpid
#include <cstring>
#include <iostream>
using namespace std;

// timer periods
#define TIM_CAM_FRAME_SECS	(5)
#define TIM_CAM_PROC_SECS	(0)
#define TIM_LAMP_ON_SECS	(10)

// minimum lamp bright - pwm value(0 - 100)
#define MIN_BRIGHT_PWM		(50)

// bad solution
static volatile bool IDReceived = false;

CLocalSystem* CLocalSystem::thisPtr = NULL;

Command_t CLocalSystem::loraCmdList[] =
{
	{"ID", idCb},
	{"LAMP", lampCb},
	{0,0}
};

CLocalSystem::CLocalSystem() :
	lamp(),
	lora(433, GATEWAY_ADDR, LS_ADDR),
	camera("video0"),
	park(),
	
	timCamFrame(TIM_CAM_FRAME_SECS, timHandler),
	timCamProc(TIM_CAM_PROC_SECS, timHandler),
	timLampOn(TIM_LAMP_ON_SECS, timHandler, false), // set non-periodic timer

	loraParser(loraCmdList, ";")
{
	if(pthread_mutex_init(&mutRecvSensors, NULL) != 0)
		panic("CLS::CLocalSystem(): Mutex init");

	if(pthread_mutex_init(&mutCamFrame, NULL) != 0)
		panic("CLS::CLocalSystem(): Mutex init");

	if(pthread_cond_init(&condRecvSensors, NULL) != 0)
		panic("CLS::CLocalSystem(): Condition variable init");	

	if(pthread_cond_init(&condCamFrame, NULL) != 0)
		panic("CLS::CLocalSystem(): Condition variable init");	

	msgqSensors = mq_open(MSGQ_NAME, O_RDWR | O_CREAT | O_NONBLOCK, NULL);
	if(msgqSensors == (mqd_t)-1)
		panic("CLS::CLocalSystem(): Creating message queue");

	if(pthread_create(&tLoraRecv_id, NULL, tLoraRecv, this) != 0)
		panic("CLS::CLocalSystem(): pthread_create");

	if(pthread_create(&tRecvSensors_id, NULL, tRecvSensors, this) != 0)
		panic("CLS::CLocalSystem(): pthread_create");

	if(pthread_create(&tParkDetection_id, NULL, tParkDetection, this) != 0)
		panic("CLS::CLocalSystem(): pthread_create");

	lora.init(2);
	
	// this pointer for static member functions use -> timer handler
	thisPtr = this;
}

CLocalSystem::~CLocalSystem()
{

}

void CLocalSystem::timLampOnISR()
{
	stringstream debug_msg;
	debug_msg << "[CLS::timLampOnISR] Sending (LAMP;MIN)";
	DEBUG_MSG(debug_msg.str());

	// turn off lamp
	lamp.setBrightness(MIN_BRIGHT_PWM);

	if(IDReceived)
		lora.push("LAMP;MIN");
}

void CLocalSystem::timCamFrameISR()
{
	// DEBUG_MSG("[CLS::timCamFrameISR] Signal tParkDetection");
	pthread_cond_signal(&condCamFrame);
}

void CLocalSystem::timCamProcISR()
{
	// DEBUG_MSG("[CLS::timCamProcISR] Camera processing taking too much time");
}

void CLocalSystem::timHandler(union sigval arg)
{
	if(!thisPtr)
		panic("CLS::timHandler(): thisPtr not defined");

	int id = arg.sival_int;
	// DEBUG_MSG("[CLS::timHandler] handling timer[" << id << "] timeout...");

	// cannot do switch statement since tim*.id is not a compile time constant
	if(id == thisPtr->timCamFrame.id)
	{
		thisPtr->timCamFrameISR();
	}
	else if(id == thisPtr->timCamProc.id)
	{
		thisPtr->timCamProcISR();
	}
	else if(id == thisPtr->timLampOn.id)
	{
		thisPtr->timLampOnISR();
	}
	else
	{
		ERROR_MSG("ERROR> [CLS::timHandler] unexpected timer event");
	}
}

#include <bcm2835.h>
void CLocalSystem::run()
{
	// start camera frame timer
	timCamFrame.start();

	// set signal handler for dSensors signal
	signal(SIG_dSENSORS, sigHandler);
	signal(SIGINT, sigHandler);

	// join lora threads
	lora.run(RUN_NONBLOCK);

	// send CRQ - connection request to the remote system. Awaits its response,
	// giving this local system a "virtual address" to be used in all comms.
	do
	{
		lora.send("CRQ");
		bcm2835_delay(2000);
	}
	while(IDReceived == false);

	// wait for <id> command using tLoraRecv thread
	// this ID will be used in every communication from that moment o

	pthread_join(tLoraRecv_id, NULL);
	pthread_join(tRecvSensors_id, NULL);
	pthread_join(tParkDetection_id, NULL);
}

void CLocalSystem::sigHandler(int sig)
{
	stringstream debug_msg;
	switch(sig)
	{
		case SIG_dSENSORS:
			// DEBUG_MSG("[CLS::sigHandler] caught SIG_dSENSORS");
			pthread_cond_signal(&thisPtr->condRecvSensors);
			break;

		case SIGINT:
			debug_msg.str("");
			debug_msg << "[CLS::sigHandler] caught SIGINT";
			DEBUG_MSG(debug_msg.str());

			// closing the queue
    		mq_close(thisPtr->msgqSensors);

    		// remove msgq from the system
		    if (mq_unlink(MSGQ_NAME) == -1)
	   			panic("Removing queue error");
    		
    		debug_msg.str("");
			debug_msg << "[CLS::sigHandler] closing...";
			DEBUG_MSG(msg.str());

			exit(0);
		default:
			debug_msg.str("");
			debug_msg << "[CLS::sigHandler] caught unexpected signal";
			DEBUG_MSG(msg.str());
	}
}

int CLocalSystem::idCb(int argc, char *argv[])
{

	if(argc != 2)
	{
		DEBUG_MSG("[CLS::idCb] Usage: ID;<id>");
		return -1;
	}
 	
 	int id = atoi(argv[1]);
 	
 	thisPtr->lora.setSrcAddr(id);
 	IDReceived = true;

 	stringstream debug_msg;
	debug_msg << "[CLS::idCb] Lamppost ID = " << id;
	DEBUG_MSG(debug_msg.str());

	return 0;
}

int CLocalSystem::lampCb(int argc, char *argv[])
{
	if(argc != 2)
	{
		DEBUG_MSG("[CLS::lampCb] Usage: LAMP;<state>");
		return -1;
	}

	// make sure that argv[1], that is, the lamp state, is valid
	parseSensorsCmd(argv[1]);

	stringstream debug_msg;
	debug_msg << "[CLS::lampCb] Lamp state set at " << argv[1];
	DEBUG_MSG(debug_msg.str());
	return 0;
}

void *CLocalSystem::tLoraRecv(void *arg)
{
	// get CLocalSystem instance
	CLocalSystem *c = reinterpret_cast<CLocalSystem*>(arg);
	string msg;
	LoRaError err;

	while(c)
	{
		// message was received?
		err = static_cast<LoRaError>(c->lora.recv(msg));
		if(err == LoRaError::MSGOK)
		{
			c->loraParser.parse(msg.c_str());

			stringstream debug_msg;
			debug_msg << "[CLS::tLoraRecv] Received[" << msg << "]";
			DEBUG_MSG(debug_msg.str());
		}

	}

	DEBUG_MSG("[CLS::tLoraRecv] exiting thread");
	return NULL;
}

// max length of a message queue
#define MAX_MSG_LEN_R     10000

void *CLocalSystem::tRecvSensors(void *arg)
{
	// get CLocalSystem instance
	CLocalSystem *c = reinterpret_cast<CLocalSystem*>(arg);
	int err = 0;
	char msg[MAX_MSG_LEN_R];

	// send PID to dSensors
	string pid_str = to_string(getpid());

	stringstream debug_msg;
	debug_msg << "[CLS::tRecvSensors] Sending PID[" << pid_str << "]";
	DEBUG_MSG(debug_msg.str());
	
	do
	{
		err = mq_send(c->msgqSensors, pid_str.c_str(), pid_str.length()+1, 1);
		if(err == -1)
		{
			err = errno;
			if(err != EAGAIN)
				panic("In mq_send()");
		}
	} while(err == EAGAIN);
	

	// make sure that mq_receive does not happen before dSensors read the msg
	mq_attr msgqAttr;
   	// wait while there are messages in msgq
   	DEBUG_MSG("[CLS::tRecvSensors] Waiting for dSensors to read PID...");
   	
   	do
   	{
   		if( mq_getattr(c->msgqSensors, &msgqAttr) == -1)
   			panic("In mq_getattr()");
   	}
   	while(msgqAttr.mq_curmsgs != 0);
   	DEBUG_MSG("[CLS::tRecvSensors] PID was read.");

   	// clear message - avoid bad content
	memset(msg, 0, sizeof(msg));

	int ret;
 
	while(c)
	{
		pthread_mutex_lock(&c->mutRecvSensors);

		ret = mq_receive(c->msgqSensors, msg, MAX_MSG_LEN_R, NULL);

		if(ret == -1)
		{ 
			// get error from errno
			err = errno;

			// error not expected
			if(err != EAGAIN)
				panic("In mq_receive()");

			// else, message queue is empty
			// DEBUG_MSG("[CLS::tRecvSensors] Waiting for condRecvSensors...");
			pthread_cond_wait(&c->condRecvSensors, &c->mutRecvSensors);
			// DEBUG_MSG("[CLS::tRecvSensors] Im awake!");
			pthread_mutex_unlock(&c->mutRecvSensors);
		}
		else
		{
			// else, messages to read from dSensors
			// DEBUG_MSG("[CLS::tRecvSensors] received cmd[" << string(msg) << "]");
			msg[ret] = '\0';

			int err = parseSensorsCmd(msg);
			if((err == 0) && IDReceived)
			{
				// send only if ID has been received
				// send to remote system an update on this lamppost status
				string loraMsg = "LAMP;" + string(msg);
				c->lora.push(loraMsg);

				debug_msg.str("");
				debug_msg << "[CLS::tRecvSensors] Sending (" << loraMsg << ")";
				DEBUG_MSG(debug_msg.str());
			}
			
			pthread_mutex_unlock(&c->mutRecvSensors);
		}
	}

	DEBUG_MSG("[CLS::tRecvSensors] exiting thread");
	return NULL;
}

void *CLocalSystem::tParkDetection(void *arg)
{
	// get CLocalSystem instance
	CLocalSystem *c = reinterpret_cast<CLocalSystem*>(arg);
	unsigned vacantsNum = 0;
	unsigned oldVacantsNum = 0;

	pthread_mutex_lock(&c->mutCamFrame);
	// capture frame
	c->camera.captureFrame();
	pthread_mutex_unlock(&c->mutCamFrame);

	// get parking outline from captured frame
	c->park.getOutline();

	while(c)
	{
		pthread_mutex_lock(&c->mutCamFrame);
		// DEBUG_MSG("[CLS::tParkDetection] Waiting for condCamFrame...");
		pthread_cond_wait(&c->condCamFrame, &c->mutCamFrame);
		// DEBUG_MSG("[CLS::tParkDetection] Im awake!");
		
		c->camera.captureFrame();
		pthread_mutex_unlock(&c->mutCamFrame);

		// start camera processing timer
		// used to detect if processing is taking too much time
		// c->timCamProc.start();

		// get vacants number
		vacantsNum = c->park.calcVacants();
		// DEBUG_MSG("[CLS::tParkDetection] vacantsNum: " << vacantsNum);

		// new number of vacants?
		if(vacantsNum != oldVacantsNum)
		{
			// send only if ID has been received
			if(IDReceived)
			{
				// send update info to remote system
				string loraMsg = "PARK;" + to_string(vacantsNum);
				c->lora.push(loraMsg);

				stringstream debug_msg;
				debug_msg << "[CLS::tParkDetection] Sending (" << loraMsg << ")";
				DEBUG_MSG(debug_msg.str());
			}
			oldVacantsNum = vacantsNum;
		}

		// stop camera processing timer
		// c->timCamProc.stop();
	}
	
	return NULL;
}

void CLocalSystem::lampOnCb(uint8_t pwm)
{
	lampAllCb(pwm);
	// start timeout
	thisPtr->timLampOn.start();	
	// DEBUG_MSG("[CLS::lampOnCb] Lamp set PWM(" << (int)pwm << ")");
}

void CLocalSystem::lampOffCb(uint8_t pwm)
{
	//setBrightness(pwm)
	lampAllCb(pwm);

	// stop timer
	thisPtr->timLampOn.stop();
}

void CLocalSystem::lampAllCb(uint8_t pwm)
{
	thisPtr->lamp.setBrightness(pwm);
	// DEBUG_MSG("[CLS::lampAllCb] Lamp set PWM(" << (int)pwm << ")");
}

cmdSensors_t CLocalSystem::cmdSensorsList[] = 
{
	{"ON"	,PWM_MAX, lampOnCb},
	{"OFF"	,PWM_OFF, lampOffCb},
	{"MIN"	,MIN_BRIGHT_PWM, lampAllCb},
	{"FAIL"	,PWM_OFF, lampOffCb},
	{0,0,0}
};

int CLocalSystem::parseSensorsCmd(char *str)
{
	cmdSensors_t *p = cmdSensorsList;

	while(p->cmd)
	{
		if(strcmp(str, p->cmd) == 0)
			break;
		p++;
	}

	if((p->cmd) == 0)
	{
		ERROR_MSG("ERROR> [parseSensorsCmd] Received unexpected command");
		return -1;
	}

	// DEBUG_MSG("[CLS::lampAllCb] Lamp " << p->cmd);
	p->cb(p->pwm);
	return 0;
}
#include "CLocalSystem.h"
#include "utils.h" // panic
#include "debug.h"

#include <unistd.h> // getpid
#include <cstring>
#include <iostream>
using namespace std;

#define GATEWAY_ADDR 	(uint8_t)(0xcc)	// destination address
#define LS_ADDR			(uint8_t)(0xbb) // local address
#define MSGQ_NAME "/dsensors"

// timer periods
#define TIM_CAM_FRAME_SECS	(0)
#define TIM_CAM_PROC_SECS	(0)
#define TIM_LAMP_ON_SECS	(5)

// minimum lamp bright - pwm value(0 - 100)
#define MIN_BRIGHT_PWM		(50)

CLocalSystem* CLocalSystem::thisPtr = NULL;

static Command_t loraCmdList[] =
{
	{"ID", idCb},
	{0,0}
};

CLocalSystem::CLocalSystem() :
	lamp(),
	lora(433, GATEWAY_ADDR, LS_ADDR),
	camera("video0"),
	
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

	// this pointer for static member functions use -> timer handler
	thisPtr = this;
}

CLocalSystem::~CLocalSystem()
{

}

void CLocalSystem::timLampOnISR()
{
	DEBUG_MSG("[CLS::timLampOnISR] Lamp at minimum bright");
	// turn off lamp
	lamp.setBrightness(MIN_BRIGHT_PWM);
}

void CLocalSystem::timCamFrameISR()
{
	DEBUG_MSG("[CLS::timCamFrameISR] Signal tParkDetection");
	pthread_cond_signal(&condCamFrame);
}

void CLocalSystem::timCamProcISR()
{
	DEBUG_MSG("[CLS::timCamProcISR] Camera processing taking too much time");
}

void CLocalSystem::timHandler(union sigval arg)
{
	if(!thisPtr)
		panic("CLS::timHandler(): thisPtr not defined");

	int id = arg.sival_int;
	DEBUG_MSG("[CLS::timHandler] handling timer[" << id << "] timeout...");

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
		ERROR_MSG("[CLS::timHandler] unexpected timer event");
	}
}

void CLocalSystem::run()
{
	// start camera frame timer
	timCamFrame.start();

	// >>>>>>>>>>>>>>>>>>>>>>>>>>>>> set prio
	lora.init(2);

	// set signal handler for dSensors signal
	signal(SIGUSR1, sigHandler);
	signal(SIGINT, sigHandler);

	// send CRQ - connection request to the remote system. Awaits its response,
	// giving this local system a "virtual address" to be used in all comms.
	lora.push("CRQ");
	// wait for <id> using tLoraRecv thread
	// this ID will be used in every communication from now on

	// join lora threads
	lora.run();

	pthread_join(tLoraRecv_id, NULL);
	pthread_join(tRecvSensors_id, NULL);
	pthread_join(tParkDetection_id, NULL);
}

void CLocalSystem::sigHandler(int sig)
{
	switch(sig)
	{
		case SIGUSR1:
			DEBUG_MSG("[CLS::sigHandler] caught SIGUSR1");
			pthread_cond_signal(&thisPtr->condRecvSensors);
			break;

		case SIGINT:
			DEBUG_MSG("[CLS::sigHandler] caught SIGINT");
			// closing the queue
    		mq_close(thisPtr->msgqSensors);

    		// remove msgq from the system
		    if (mq_unlink(MSGQ_NAME) == -1)
	   			panic("Removing queue error");
    		
    		DEBUG_MSG("[CLS::sigHandler] closing...");
			exit(0);
		default:
			ERROR_MSG("[CLS::sigHandler] caught unexpected signal");
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
 	
 	lora.setSrcAddr(id);
 	DEBUG_MSG("[CLS::idCb] Lamppost ID as " << id);
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
			cout << "Received[" << msg << "]" << endl;
		}
	}

	DEBUG_MSG("[CLS::tLoraRecv] exiting thread");
	return NULL;
}

struct cmdSensors_t
{
	char const *cmd;
	uint8_t pwm;
};

static cmdSensors_t cmdSensorsList[] = 
{
	{"ON"	,PWM_MAX},
	{"OFF"	,PWM_OFF},
	{"MIN"	,MIN_BRIGHT_PWM},
	{"FAIL"	,PWM_OFF},
	{0,0}
};

static uint8_t parseSensorsCmd(char *str)
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
		ERROR_MSG("[parseSensorsCmd] Unexpected command from dSensors");
		return -1;
	}

	return p->pwm;
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
	DEBUG_MSG("[CLS::tRecvSensors] Sending PID[" << pid_str << "]");
	if(mq_send(c->msgqSensors, pid_str.c_str(), pid_str.length(), 1) != 0)
		panic("In mq_send()");

	// make sure that mq_receive does not happen before dSensors read the msg
	mq_attr msgqAttr;
   	// wait while there are messages in msgq
   	DEBUG_MSG("[CLS::tRecvSensors] Waiting for dSensors to read PID...");
   	do
   	{
   		mq_getattr(c->msgqSensors, &msgqAttr);
   	}
   	while(msgqAttr.mq_curmsgs != 0);

   	// clear message - avoid bad content
	memset(msg, 0, sizeof(msg));
	
	// // wait for ID attribution to this lamppost
	// while(c->lora.getLocalAddr() == -1)
	// 	;

	while(c)
	{
		pthread_mutex_lock(&c->mutRecvSensors);

		if(mq_receive(c->msgqSensors, msg, MAX_MSG_LEN_R, NULL) == -1)
		{ 
			// get error from errno
			err = errno;

			// error not expected
			if(err != EAGAIN)
				panic("In mq_receive()");

			// else, message queue is empty
			DEBUG_MSG("[CLS::tRecvSensors] Waiting for condRecvSensors...");
			pthread_cond_wait(&c->condRecvSensors, &c->mutRecvSensors);
			DEBUG_MSG("[CLS::tRecvSensors] Im awake!");
			pthread_mutex_unlock(&c->mutRecvSensors);
		}
		else
		{
			// else, messages to read from dSensors
			DEBUG_MSG("[CLS::tRecvSensors] received cmd[" << string(msg) << "]");

			int cmdPwm = parseSensorsCmd(msg);
			DEBUG_MSG("[CLS::tRecvSensors] Setting lamp PWM to[" << cmdPwm << "]");
			c->lamp.setBrightness(cmdPwm);

			if(cmdPwm == PWM_MAX)
				// Start lamp On timeout. When this ends, the lamp PWM is set
				// to MIN_BRIGHT_PWM
				c->timLampOn.start();

			string loraMsg = "LAMP;" + msg;
			DEBUG_MSG("[CLS::tRecvSensors] Sending (" << loraMsg << ")");
			// send to remote system an update on this lamppost status
			c->lora.push(loraMsg);

			// clear message
			memset(msg, 0, sizeof(msg));
			
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
	// camera.capture();
	pthread_mutex_unlock(&c->mutCamFrame);

	// get parking outline from captured frame
	// park.getOutline(frame);

	// wait for ID attribution to this lamppost
	// while(c->lora.getLocalAddr() == -1)
	// 	;
	
	while(c)
	{
		pthread_mutex_lock(&c->mutCamFrame);
		DEBUG_MSG("[CLS::tParkDetection] Waiting for condCamFrame...");
		pthread_cond_wait(&c->condCamFrame, &c->mutCamFrame);
		DEBUG_MSG("[CLS::tParkDetection] Im awake!");
		
		// camera.capture();
		pthread_mutex_unlock(&c->mutCamFrame);

		// start camera processing timer
		// used to detect if processing is taking too much time
		c->timCamProc.start();
		// get vacants number
		// vacantsNum = c->park.getVacants(frame);

		// new number of vacants?
		if(vacantsNum != oldVacantsNum)
		{
			// send update info to remote system
			string loraMsg = "PARK;" + vacantsNum;
			c->lora.push(loraMsg);
			oldVacantsNum = vacantsNum;
		}

		// stop camera processing timer
		c->timCamProc.stop();
	}
	
	return NULL;
}

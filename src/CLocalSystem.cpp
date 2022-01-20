#include "CLocalSystem.h"
#include "utils.h" // panic
#include "debug.h"

#include <unistd.h>
#include <cstring>
#include <iostream>
using namespace std;

#define TIM_CAM_PROC 1
#define TIM_CAM_FRAME 2
#define TIM_LAMP_ON 3

CLocalSystem* CLocalSystem::thisPtr = NULL;

static Command_t loraCmdList[] =
{
	{0,0}
};

CLocalSystem::CLocalSystem() :
	lamp(),
	lora(433, GATEWAY_ADDR, LS_ADDR),
	
	timCamFrame(TIM_CAM_FRAME_SECS, timCamFrameHandler),
	timCamProc(TIM_CAM_PROC_SECS, timCamProcHandler),
	timLampOnSecs(TIM_LAMP_ON_SECS, timLampOnHandler, false), // timer not periodic

	loraParser(loraCmdList, " ")
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

	thisPtr = this;
}

CLocalSystem::~CLocalSystem()
{

}


void CLocalSystem::timLampOnHandler(union sigval arg)
{
	if(thisPtr)
		thisPtr->timer_handler(arg.sival_int);
}

void CLocalSystem::timCamFrameHandler(union sigval arg)
{
	if(thisPtr)
		thisPtr->timer_handler(arg.sival_int);
}

void CLocalSystem::timCamProcHandler(union sigval arg)
{
	if(thisPtr)
		thisPtr->timer_handler(arg.sival_int);
}

void CLocalSystem::timer_handler(int tim_num)
{
	DEBUG_MSG("[CLS::timer_handler] handling timer[" << tim_num << "] timeout");

	switch(tim_num)
	{
		// case (timCamFrame.id):
		case TIM_CAM_FRAME:
			DEBUG_MSG("[CLS::timer_handler] Signal tParkDetection");
			pthread_cond_signal(&condCamFrame);
			break;

		case TIM_LAMP_ON:
			DEBUG_MSG("[CLS::timer_handler] Turn off lamp");
			// turn off lamp
			lamp.setBrightness(MIN_BRIGHT_PWM);
			break;

		case TIM_CAM_PROC:
			DEBUG_MSG("[CLS::timer_handler] Signal xxx");
			// pthread_cond_signal(&condCamFrame);
			break;

		default:
			ERROR_MSG("[CLS::timer_handler] unexpected timer event");
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
			// pthread_cond_signal(&c->condRecvSensors);
			break;

		default:
			ERROR_MSG("[CLS::sigHandler] caught unexpected signal");
	}
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
			cout << "[CLS::tLoraRecv] received[" << msg << "]" << endl;
			// c->lora.push(msg); // test echoing
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
	{"ON",100},
	{"OFF",0},
	{"MIN",MIN_BRIGHT_PWM},
	{"FAIL", 0},
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

	// DEBUG_MSG("[parseSensorsCmd] cmd(" << p->cmd << ") has pwm(" << p->pwm << ")");
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
	// possible fix...
	// wait while there are messages in msgq
	// ..........
	
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
		}
		else
		{
			// else, messages to read from dSensors
			pthread_mutex_unlock(&c->mutRecvSensors);
			DEBUG_MSG("[CLS::tRecvSensors] Sending (" << "LAMP " + string(msg) << ")");
			c->lora.push(string("LAMP ") + msg);

			int cmdPwm = parseSensorsCmd(msg);
			DEBUG_MSG("[CLS::tRecvSensors] Setting lamp PWM to[" << cmdPwm << "]");
			c->lamp.setBrightness(cmdPwm);
		}
	}
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
			c->lora.push("PARK "+ vacantsNum);
			oldVacantsNum = vacantsNum;
		}

		// stop camera processing timer
		c->timCamProc.stop();
	}
	
	return NULL;
}

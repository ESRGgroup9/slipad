#include "CLocalSystem.h"
#include "utils.h" // panic
#include "debug.h"
#include <unistd.h>

#include <iostream>
using namespace std;

static pthread_cond_t condCamFrame;

CLocalSystem::CLocalSystem() :
	lamp(TIM_LAMP_ON_SECS),
	lora(433, GATEWAY_ADDR, LS_ADDR),
	
	timCamFrame(TIM_CAM_FRAME_SECS, timCamFrameHandler),
	timCamProc(TIM_CAM_PROC_SECS, timCamProcHandler)
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
}

CLocalSystem::~CLocalSystem()
{

}

void CLocalSystem::timCamFrameHandler(union sigval arg)
{
	DEBUG_MSG("[CLS::timCamFrameHandler] Signal tParkDetection");
	pthread_cond_signal(&condCamFrame);
}

void CLocalSystem::timCamProcHandler(union sigval arg)
{
	DEBUG_MSG("[CLS::timCamProcHandler] Signal xxx");
	// pthread_cond_signal(&condCamFrame);
}

void CLocalSystem::run()
{
	// send PID to dSensors
	string pid_str = to_string(getpid());
	DEBUG_MSG("[CLS::run] Sending PID[" << pid_str << "]");

	if(mq_send(msgqSensors, pid_str.c_str(), pid_str.length(), 1) != 0)
		panic("In mq_send()");

	// start camera frame timer
	timCamFrame.start();

	// >>>>>>>>>>>>>>>>>>>>>>>>>>>>> set prio
	lora.init(2);
	// join lora threads
	lora.run();

	pthread_join(tLoraRecv_id, NULL);
	pthread_join(tRecvSensors_id, NULL);
	pthread_join(tParkDetection_id, NULL);
}

void *CLocalSystem::tLoraRecv(void *arg)
{
	// get CLocalSystem instance
	CLocalSystem *c = reinterpret_cast<CLocalSystem*>(arg);
	string msg;
	LoRaError err;

	DEBUG_MSG("[CLS::tLoraRecv] entering thread");
	c->lora.push("hello");

	while(c)
	{
		// message was received?
		err = static_cast<LoRaError>(c->lora.recv(msg));

		if(err == LoRaError::MSGOK)
		{
			// parse(msg);
			cout << "[CLS::tLoraRecv] received[" << msg << "]" << endl;
			// c->lora.push(msg); // test echoing
		}
	}

	DEBUG_MSG("[CLS::tLoraRecv] exiting thread");
	return NULL;
}

void *CLocalSystem::tRecvSensors(void *arg)
{
	// get CLocalSystem instance
	CLocalSystem *c = reinterpret_cast<CLocalSystem*>(arg);
	
	while(c)
		;

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
		pthread_cond_wait(&condCamFrame, &c->mutCamFrame);
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

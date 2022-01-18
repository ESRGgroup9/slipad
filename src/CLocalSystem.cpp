#include "CLocalSystem.h"
#include "utils.h" // panic
#include "debug.h"

CLocalSystem::CLocalSystem() :
	lora(433, GATEWAY_ADDR, LS_ADDR),
	// lamp(TIM_LAMP_ON_SECS),

	timCamFrameSecs(TIM_CAM_FRAME_SECS),
	timCamProcSecs(TIM_CAM_PROC_SECS)
{
	if(pthread_mutex_init(&mutRecvSensors, NULL) != 0)
		panic("CLocalSystem::CLocalSystem(): Mutex init");

	if(pthread_mutex_init(&mutCamFrame, NULL) != 0)
		panic("CLocalSystem::CLocalSystem(): Mutex init");

	if(pthread_cond_init(&condRecvSensors, NULL) != 0)
		panic("CLocalSystem::CLocalSystem(): Condition variable init");	

	if(pthread_cond_init(&condCamFrame, NULL) != 0)
		panic("CLocalSystem::CLocalSystem(): Condition variable init");	

	msgqSensors = mq_open(MSGQ_NAME, O_RDONLY | O_CREAT | O_NONBLOCK, NULL);
	if(msgqSensors == (mqd_t)-1)
		panic("CLocalSystem::CLocalSystem(): Creating message queue");

	if(pthread_create(&tLoraRecv_id, NULL, tLoraRecv, this) != 0)
		panic("CLocalSystem::CLocalSystem(): pthread_create");

	if(pthread_create(&tRecvSensors_id, NULL, tRecvSensors, this) != 0)
		panic("CLocalSystem::CLocalSystem(): pthread_create");

	if(pthread_create(&tParkDetection_id, NULL, tParkDetection, this) != 0)
		panic("CLocalSystem::CLocalSystem(): pthread_create");
}

CLocalSystem::~CLocalSystem()
{

}

void CLocalSystem::run()
{
	// setTimer(timCamFrame) >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	lora.init(2);// >>>>>>>>>>>>>>>>>>>>>>>>>>>>> set prio
	// join lora threads
	lora.run();

	pthread_join(tLoraRecv_id, NULL);
	pthread_join(tRecvSensors_id, NULL);
	pthread_join(tParkDetection_id, NULL);
}

void *CLocalSystem::tLoraRecv(void *arg)
{
	// get CLocalSystem instance
	CLocalSystem *clsys = reinterpret_cast<CLocalSystem*>(arg);
	string msg;
	LoRaError err;

	DEBUG_MSG("[tLoraRecv] entering thread");
	clsys->lora.push("hello");

	while(clsys)
	{
		// message was received?
		err = static_cast<LoRaError>(clsys->lora.recv(msg));

		if(err == LoRaError::MSGOK)
		{
			// parse(msg);
			DEBUG_MSG("[tLoraRecv] received[" << msg << "]");
			clsys->lora.push(msg);
		}
	}

	DEBUG_MSG("[tLoraRecv] exiting thread");
	return NULL;
}

void *CLocalSystem::tRecvSensors(void *arg)
{
	// get CLocalSystem instance
	CLocalSystem *clsys = reinterpret_cast<CLocalSystem*>(arg);
	
	while(clsys)
		;

	return NULL;
}

void *CLocalSystem::tParkDetection(void *arg)
{
	// get CLocalSystem instance
	CLocalSystem *clsys = reinterpret_cast<CLocalSystem*>(arg);
	
	while(clsys)
		;
	
	return NULL;
}

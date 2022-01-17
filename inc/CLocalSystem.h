#ifndef __CLOCALSYSTEM_H__
#define __CLOCALSYSTEM_H__

#include "CLamp.h"
#include "CLoraComm.h"
#include "CCamera.h"
#include "CParkDetection.h"

#include <pthread.h>
#include <mqueue.h> // mqd_t

#define GATEWAY_ADDR 	0
#define LS_ADDR			0

#define TIM_CAM_FRAME_SECS	0
#define TIM_CAM_PROC_SECS	0

#define MSGQ_NAME "/dsensors"

class CLocalSystem 
{
public:
	CLocalSystem();
	~CLocalSystem();

	void run();

private:
	static void *tLoraRecv(void*);
	static void *tRecvSensors(void*);
	static void *tParkDetection(void*);

private:
	CLamp lamp;
	CLoraComm lora;
	CCamera camera;
	CParkDetection park;

	pthread_t tLoraRecv_id;
	pthread_t tRecvSensors_id;
	pthread_t tParkDetection_id;

	mqd_t msgqSensors;
	pthread_mutex_t mutRecvSensors;
	pthread_cond_t condRecvSensors;

	pthread_mutex_t mutCamFrame;
	pthread_cond_t condCamFrame;

	const int timCamFrameSecs;
	const int timCamProcSecs;
};
// End CLocalSystem class definition

#endif // !__CLOCALSYSTEM_H__

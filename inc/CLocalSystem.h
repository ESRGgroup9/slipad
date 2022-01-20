#ifndef __CLOCALSYSTEM_H__
#define __CLOCALSYSTEM_H__

#include "CLamp.h"
#include "CLoraComm.h"
#include "CCamera.h"
#include "CParkDetection.h"
#include "timer.h"
#include "parser.h"

#include <pthread.h>
#include <mqueue.h> // mqd_t
 
#define GATEWAY_ADDR 	(uint8_t)(0xcc)	// destination address
#define LS_ADDR			(uint8_t)(0xbb) // local address

// timer periods
#define TIM_CAM_FRAME_SECS	(5)
#define TIM_CAM_PROC_SECS	(0)
#define TIM_LAMP_ON_SECS	(0)

#define MIN_BRIGHT_PWM		(50)

#define MSGQ_NAME "/dsensors"

class CLocalSystem 
{
public:
	CLocalSystem();
	~CLocalSystem();

	void run();

private:
	static void sigHandler(int sig);

	static void *tLoraRecv(void*);
	static void *tRecvSensors(void*);
	static void *tParkDetection(void*);

private:
	static CLocalSystem* thisPtr;
	static void timCamFrameHandler(union sigval arg);
	static void timCamProcHandler(union sigval arg);
	static void timLampOnHandler(union sigval arg);

	void timer_handler(int tim_num);
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
	pthread_mutex_t mutCamFrame;
	pthread_cond_t condRecvSensors;
	pthread_cond_t condCamFrame; 

	Timer timCamFrame;
	Timer timCamProc;
	Timer timLampOnSecs;
	
	Parser loraParser;
};
// End CLocalSystem class definition

#endif // !__CLOCALSYSTEM_H__

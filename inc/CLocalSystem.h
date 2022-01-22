#ifndef __CLOCALSYSTEM_H__
#define __CLOCALSYSTEM_H__

#include "CLamp.h"
#include "CLoraComm.h"
//#include "CCamera.h"
#include "CParkDetection.h"
#include "timer.h"
#include "parser.h"

#include <pthread.h>
#include <mqueue.h> // mqd_t

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
	static CLocalSystem* thisPtr;
	
	static void sigHandler(int sig);
	static void timer_handler(union sigval arg);

	void timCamFrameISR();
	void timCamProcISR();
	void timLampOnISR();

private:
	CLamp lamp;
	CLoraComm lora;
	//CCamera camera;
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
	Timer timLampOn;
	
	Parser loraParser;
};
// End CLocalSystem class definition

#endif // !__CLOCALSYSTEM_H__

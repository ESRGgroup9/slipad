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

struct cmdSensors_t
{
	char const *cmd;
	uint8_t pwm;
	void (*cb)(uint8_t);
};

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
	static int idCb(int argc, char *argv[]);
	static int lampCb(int argc, char *argv[]);
	
	static int parseSensorsCmd(char *str);
	static void lampOnCb(uint8_t pwm);
	static void lampOffCb(uint8_t pwm);
	static void lampAllCb(uint8_t pwm);

	static void sigHandler(int sig);
	static void timHandler(union sigval arg);

	void timCamFrameISR();
	void timCamProcISR();
	void timLampOnISR();

private:
	CLamp lamp;
	CLoraComm lora;
	CCamera camera;
	CParkDetection park;

	pthread_t tLoraRecv_id;
	pthread_t tRecvSensors_id;
	pthread_t tParkDetection_id;

	pthread_mutex_t mutRecvSensors;
	pthread_mutex_t mutCamFrame;
	
	pthread_cond_t condRecvSensors;
	pthread_cond_t condCamFrame; 

	Timer timCamFrame;
	Timer timCamProc;
	Timer timLampOn;
	
	mqd_t msgqSensors;
	Parser loraParser;
	static cmdSensors_t cmdSensorsList[];
	static Command_t loraCmdList[];
};
// End CLocalSystem class definition

#endif // !__CLOCALSYSTEM_H__

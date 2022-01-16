#ifndef __CCOMMUNICATION_H__
#define __CCOMMUNICATION_H__

#include <pthread.h>
#include <string>
#include <queue>
#include <bcm2835.h>

#define LORA_SS_PIN        RPI_V2_GPIO_P1_11
#define LORA_RESET_PIN     RPI_GPIO_P1_22
#define LORA_DIO0_PIN      RPI_GPIO_P1_18

enum class ConnStatus
{
	CREATED = 0,
	ONLINE,
	ERROR,
	CLOSED
};

class CCommunication
{
public:
	CCommunication();
	~CCommunication();

	void init(int tprio);
	void run(void);
	ConnStatus getStatus(void) const;

	void push(std::string msg);
	int send(std::string msg);
	std::string recv(void);

protected:
	static void *tSend(void*);
	virtual std::string recvFunc(void) = 0;
	virtual int sendFunc(std::string msg) = 0;
	
	ConnStatus status;

private:
	std::queue<std::string> TxMsgs;

	pthread_t tSend_id;
	pthread_mutex_t mutComms;
	pthread_mutex_t mutTxMsgs;
	pthread_cond_t condtSend;
};
// End CCommunication class definition

#endif // !__CCOMMUNICATION_H__

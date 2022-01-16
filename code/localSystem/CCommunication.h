#ifndef __CCOMMUNICATION_H__
#define __CCOMMUNICATION_H__

#include <string>
#include <queue>
#include <pthread.h>

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

	int push(std::string msg);
	int send(std::string msg);
	std::string recv(void);

protected:
	static void *tSend(void*);
	virtual std::string recvFunc(void) = 0;
	virtual int sendFunc(std::string msg) = 0;
	
private:
	ConnStatus status;
	std::queue<std::string> TxMsgs;

	pthread_t tSend_id;
	pthread_mutex_t mutComms;
	pthread_mutex_t mutTxMsgs;
	pthread_cond_t condtSend;
};
// End CCommunication class definition

#endif // !__CCOMMUNICATION_H__

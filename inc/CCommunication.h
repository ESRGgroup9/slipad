#ifndef __CCOMMUNICATION_H__
#define __CCOMMUNICATION_H__

#include <pthread.h>
#include <string>
#include <queue>

// connection status
enum class ConnStatus
{
	CREATED = 0,
	ONLINE,
	ERROR,
	CLOSED
};

// message status error
// enum class MsgStatus
// {
// 	MSGOK = 0, 	// message OK
// 	ENOMSGR,	// no message received
// 	ENOTME,		// Message received is not for this device
// 	EBADLMSG	// Message received lengths does not match
// };

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
	int recv(std::string &msg);

protected:
	static void *tSend(void*);		
	ConnStatus status;

	// pure virtual methods
	virtual int recvFunc(std::string &msg) = 0;
	virtual int sendFunc(std::string msg) = 0;

private:
	std::queue<std::string> TxMsgs;

	pthread_t tSend_id;
	pthread_mutex_t mutComms;
	pthread_mutex_t mutTxMsgs;
	pthread_cond_t condtSend;
};
// End CCommunication class definition

#endif // !__CCOMMUNICATION_H__

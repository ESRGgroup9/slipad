#include "CCommunication.h"
#include "utils.h"
using namespace std;

CCommunication::CCommunication()
{
	if(pthread_mutex_init(&mutTxMsgs, NULL) != 0)
		panic("CComms::CComms(): Mutex init");

	if(pthread_mutex_init(&mutComms, NULL) != 0)
		panic("CComms::CComms(): Mutex init");

	if(pthread_cond_init(&condtSend, NULL) != 0)
		panic("CComms::CComms(): Condition variable init");	

	status = ConnStatus::CREATED;
}

CCommunication::~CCommunication()
{

}

void CCommunication::init(int tprio)
{
	// >>>>>>>>>>>>>>>>>>>>>>> SET PRIO

	if(pthread_create(&tSend_id, NULL, &tSend, this) != 0)
		panic("CComms::init(): pthread_create");	
}

void CCommunication::run(void)
{

}

ConnStatus CCommunication::getStatus(void) const
{
	return status;
}

void CCommunication::push(string msg)
{
	pthread_mutex_lock(&mutTxMsgs);
	TxMsgs.push(msg);
	pthread_cond_signal(&condtSend);
	pthread_mutex_unlock(&mutTxMsgs);
}

int CCommunication::send(string msg)
{
	int ret = 0;

	pthread_mutex_lock(&mutComms);
	ret = sendFunc(msg);
	pthread_mutex_unlock(&mutComms);
	
	return ret;
}

string CCommunication::recv(void)
{
	string msg;

	pthread_mutex_lock(&mutComms);
	msg = recvFunc();
	pthread_mutex_unlock(&mutComms);
	
	return msg;
}

void *CCommunication::tSend(void *arg)
{
	// CCommunication *ccomm = static_cast<CCommunication>(arg);

	while(1)
	{
		// pthread_mutex_lock(&mutTxMsgs);

		// // is there any message queued to send?
		// if(TxMsgs.empty())
		// 	pthread_cond_wait(&condtSend);

		// // pop msg from queue
		// msg = TxMsgs.front();
		// // Removes the next element in the queue, reducing its size by one
		// TxMsgs.pop();
		// pthread_mutex_unlock(&mutTxMsgs);
		
		// send(msg);
	}

	return NULL;
}

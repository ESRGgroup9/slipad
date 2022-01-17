#include "CCommunication.h"
#include "utils.h"
#include "debug.h"

#include <iostream>
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
	if(pthread_create(&tSend_id, NULL, tSend, this) != 0)
		panic("CComms::init(): pthread_create");
}

void CCommunication::run(void)
{
	pthread_join(tSend_id, NULL);
}

ConnStatus CCommunication::getStatus(void) const
{
	return status;
}

void CCommunication::push(string msg)
{
	pthread_mutex_lock(&mutTxMsgs);
	TxMsgs.push(msg);
	DEBUG_MSG("[push] Signal condtSend");
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

int CCommunication::recv(string &msg)
{
	int err = 0;

	pthread_mutex_lock(&mutComms);
	err = recvFunc(msg);
	pthread_mutex_unlock(&mutComms);
	
	return err;
}

void *CCommunication::tSend(void *arg)
{
	// get CCommunication instance
	CCommunication *ccomm = reinterpret_cast<CCommunication*>(arg);
	string msg;

	while(ccomm)
	{
		pthread_mutex_lock(&ccomm->mutTxMsgs);

		// is there any message queued to send?
		if(ccomm->TxMsgs.empty())
		{
			DEBUG_MSG("[tSend] Waiting for condtSend...");
			pthread_cond_wait(&ccomm->condtSend, &ccomm->mutTxMsgs);
			DEBUG_MSG("[tSend] Im awake!");
		}

		// pop msg from queue
		msg = ccomm->TxMsgs.front();
		// Removes the next element in the queue, reducing its size by one
		ccomm->TxMsgs.pop();
		pthread_mutex_unlock(&ccomm->mutTxMsgs);
		
		DEBUG_MSG("[tSend] Popped(" << msg << ") [" << ccomm->TxMsgs.size() << "] msgs queued");
		// send message
		ccomm->send(msg);
		msg.clear();
	}

	return NULL;
}

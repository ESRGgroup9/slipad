#undef DEBUG

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

void CCommunication::init(int tprio)
{
	// >>>>>>>>>>>>>>>>>>>>>>> SET PRIO
	if(pthread_create(&tSend_id, NULL, tSend, this) != 0)
		panic("CComms::init(): pthread_create");
}

void CCommunication::run(bool async)
{
	if(async)
		// dont track the thread
		pthread_detach(tSend_id);
	else
		// default operation. Wait for thread termination
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
	DEBUG_MSG("[CComms::push] Pushed(" << msg << ") - Signal condtSend");
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

	// while(ccomm->status != ConnStatus::CLOSED)
	// {
	// 	pthread_mutex_lock(&ccomm->mutTxMsgs);

	// 	// is there any message queued to send?
	// 	if(ccomm->TxMsgs.empty())
	// 	{
	// 		// no messages to send. wait for condtSend
	// 		DEBUG_MSG("[CComms::tSend] Waiting for condtSend...");
	// 		pthread_cond_wait(&ccomm->condtSend, &ccomm->mutTxMsgs);
	// 		DEBUG_MSG("[CComms::tSend] Im awake!");
	// 	}

	// 	// pop msg from queue
	// 	msg = ccomm->TxMsgs.front();
	// 	// Removes the next element in the queue, reducing its size by one
	// 	ccomm->TxMsgs.pop();
	// 	pthread_mutex_unlock(&ccomm->mutTxMsgs);
		
	// 	DEBUG_MSG("[CComms::tSend] Popped(" << msg << ") - " << ccomm->TxMsgs.size() << " msgs queued");
	// 	// send message
	// 	ccomm->send(msg);
	// 	DEBUG_MSG("[CComms::tSend] Sent(" << msg << ")");
	// 	msg.clear();
	// }

	do
	{
		pthread_mutex_lock(&ccomm->mutTxMsgs);

		// is there any message queued to send?
		if(ccomm->TxMsgs.empty())
		{
			// no messages to send. wait for condtSend
			DEBUG_MSG("[CComms::tSend] Waiting for condtSend...");
			pthread_cond_wait(&ccomm->condtSend, &ccomm->mutTxMsgs);

			if(ccomm->status == ConnStatus::CLOSED)
				break;
			
			DEBUG_MSG("[CComms::tSend] Im awake!");
		}

		// pop msg from queue
		msg = ccomm->TxMsgs.front();
		// Removes the next element in the queue, reducing its size by one
		ccomm->TxMsgs.pop();
		pthread_mutex_unlock(&ccomm->mutTxMsgs);
		
		DEBUG_MSG("[CComms::tSend] Popped(" << msg << ") - " << ccomm->TxMsgs.size() << " msgs queued");
		// send message
		ccomm->send(msg);
		DEBUG_MSG("[CComms::tSend] Sent(" << msg << ")");
		msg.clear();
	}
	while(ccomm->status != ConnStatus::CLOSED);

	DEBUG_MSG("[CComms::tSend] exiting...");
	return NULL;
}

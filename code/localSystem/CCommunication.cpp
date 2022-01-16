#include "CCommunication.h"
#include "utils.h"

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

// CCommunication::~CCommunication()
// {

// }

void CCommunication::init(int tprio)
{
	// >>>>>>>>>>>>>>>>>>>>>>> SET PRIO
	cout << "Push hello1" << endl;
	TxMsgs.push("hello1");
	cout << "Push hello2" << endl;
	TxMsgs.push("hello2");
	cout << "Push hello3" << endl;
	TxMsgs.push("hello3");

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
	// get CCommunication instance
	CCommunication *ccomm = reinterpret_cast<CCommunication*>(arg);
	string msg;

	while(ccomm)
	{
		pthread_mutex_lock(&ccomm->mutTxMsgs);

		// is there any message queued to send?
		if(ccomm->TxMsgs.empty())
		{
			cout << "[tSend] Waiting for condtSend..." << endl;
			pthread_cond_wait(&ccomm->condtSend, &ccomm->mutTxMsgs);
		}

		// pop msg from queue
		msg = ccomm->TxMsgs.front();
		// Removes the next element in the queue, reducing its size by one
		ccomm->TxMsgs.pop();
		pthread_mutex_unlock(&ccomm->mutTxMsgs);
		
		cout << "[tSend] Popped(" << msg << ") [" << ccomm->TxMsgs.size() << "] msgs queued" << endl;
		// ccomm->send(msg);
		msg.clear();
	}

	return NULL;
}

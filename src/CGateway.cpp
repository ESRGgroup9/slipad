#include "CGateway.h"
#include "utils.h"

CGateway::CGateway()
{
	if(pthread_create(&tLoraRecv_id, NULL, tLoraRecv, this) != 0)
		panic("CGateway::CGateway(): pthread_create");

	if(pthread_create(&tTCPRecv_id, NULL, tTCPRecv, this) != 0)
		panic("CGateway::CGateway(): pthread_create");
}

CGateway::~CGateway()
{

}

void CGateway::run()
{

}

void *CGateway::tLoraRecv(void *arg)
{

	return NULL;
}

void *CGateway::tTCPRecv(void *arg)
{

	return NULL;
}

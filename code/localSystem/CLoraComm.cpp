#include "CLoraComm.h"
using namespace std;

CLoraComm::CLoraComm(int freqMhz, int dest, int src)
{
	init(25);
}

CLoraComm::~CLoraComm()
{

}

int CLoraComm::getLocalAddr(void) const
{
	return local_addr;
}

string CLoraComm::recvFunc(void)
{
	return "hello";
}

int CLoraComm::sendFunc(std::string msg)
{

	return 0;
}
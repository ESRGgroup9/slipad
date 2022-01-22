#ifndef __CLORACOMM_H__
#define __CLORACOMM_H__

#include "CCommunication.h"
#include <string>

class CLoraComm : public CCommunication
{
public:
	CLoraComm(int freqMhz, int dest, int src);
	~CLoraComm();

	int getLocalAddr(void) const;

protected:
	virtual int recvFunc(std::string &msg);
	virtual int sendFunc(std::string msg);

private:
	int freqMhz;
	int dest_addr;
	int local_addr;
};
// End  CLoraComm class definition

#endif // !__CLORACOMM_H__

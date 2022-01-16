#ifndef __CLORACOMM_H__
#define __CLORACOMM_H__

#include <string>
#include "CCommunication.h"

class CLoraComm : public CCommunication
{
public:
	CLoraComm(int freqMhz, int dest, int src);
	~CLoraComm();

	int getLocalAddr(void) const;

protected:
	std::string recvFunc(void);
	int sendFunc(std::string msg);

private:
	int freqMhz;
	int dest_addr;
	int local_addr;
};
// End  CLoraComm class definition

#endif // !__CLORACOMM_H__

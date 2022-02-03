#include "CFailureDetector.h"
#include "utils.h"
#include "defs.h"

#include <unistd.h> 

// #define SIGH 	(LAMPF_SIG_NUM) //SIGUSR1
#define DEV_NAME "lampf"

CFailureDetector::CFailureDetector() : CCharacterDev(DEV_NAME)
{

}

CFailureDetector::~CFailureDetector(void)
{
	CCharacterDev::Close();
}

void CFailureDetector::enable(void)
{	
	CCharacterDev::Open();
}

void CFailureDetector::disable(void)
{
	CCharacterDev::Close();
}

char CFailureDetector::read(void)
{
	char readState;

	// // read successfuly
	// if(dev < 0)
	// {
	// 	CCharacterDev::open();
	// }
	if(::read(dev, &readState, 1) == 0)
		return readState;

	// error reading 
	//panic("[Lampf] Failed to read sensor value...\n");
	return -1;
}
#include "CFailureDetector.h"
#include "utils.h"
#include "defs.h"

#define SIGH 	(LAMPF_SIG_NUM) //SIGUSR1
#define DEV_NAME "lampf"

CFailureDetector::CFailureDetector(ISR isr) : CCharacterDev(DEV_NAME)
{
	sigemptyset(&act.sa_mask);
	handler = isr;
}

CFailureDetector::~CFailureDetector(void)
{
	CCharacterDev::Close();
}

void CFailureDetector::enable(void)
{	
	CCharacterDev::Open();

	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = handler;
	
	sigaction(SIGH, &act, NULL);
}

void CFailureDetector::disable(void)
{
	CCharacterDev::Close();

	act.sa_handler = SIG_IGN;

	sigaction(SIGH, &act, NULL);
}

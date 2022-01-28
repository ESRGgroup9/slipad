#include "CFailureDetector.h"
#include "utils.h"

#define SIGH 	(SIGUSR2)
#define DEV_NAME "lampf"

CFailureDetector::CFailureDetector(ISR isr) : CCharacterDev(DEV_NAME)
{
	sigemptyset(&act.sa_mask);
	handler = isr;
}

CFailureDetector::~CFailureDetector(void)
{

}

void CFailureDetector::enable(void)
{	
	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = handler;
	
	sigaction(SIGH, &act, NULL);
}

void CFailureDetector::disable(void)
{
	act.sa_handler = SIG_IGN;

	sigaction(SIGH, &act, NULL);
}

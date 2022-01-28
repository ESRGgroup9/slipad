#include "CPir.h"
#include "utils.h"

#define SIGH 	(SIGUSR1)
#define DEV_NAME "pir"

CPir::CPir(ISR isr) : CCharacterDev(DEV_NAME)
{
	sigemptyset(&act.sa_mask);
	handler = isr; 
}

CPir::~CPir()
{

}

void CPir::enable(void)
{	
	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = handler;
	
	sigaction(SIGH, &act, NULL);
}

void CPir::disable(void)
{
	act.sa_handler = SIG_IGN;

	sigaction(SIGH, &act, NULL);
}

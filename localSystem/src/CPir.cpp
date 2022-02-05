#include "CPir.h"
#include "utils.h"
#include "defs.h"

#define SIGH 	(PIR_SIG_NUM) //SIGUSR1
#define DEV_NAME "pir"

CPir::CPir(ISR isr) : CCharacterDev(DEV_NAME)
{
	sigemptyset(&act.sa_mask);
	handler = isr; 
}

CPir::~CPir()
{
	CCharacterDev::Close();
}

void CPir::enable(void)
{	
	CCharacterDev::Open();

	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = handler;
	
	sigaction(SIGUSR1, &act, NULL);
}

void CPir::disable(void)
{
	CCharacterDev::Close();

	act.sa_handler = SIG_IGN;

	sigaction(SIGUSR1, &act, NULL);
}

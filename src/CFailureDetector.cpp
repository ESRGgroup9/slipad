#include "CFailureDetector.h"

#include <stdio.h>
// #include <fcntl.h>
// #include <unistd.h> 
// #include <sys/ioctl.h>
#include "utils.h"

#define SIGHIGH 10
#define IOCTL_PID 1

#define DEV_NAME "lampf"

CFailureDetector::CFailureDetector(ISR lampfISR) : CCharacterDev(DEV_NAME)
{
	sigemptyset(&act.sa_mask);

	handler = lampfISR;
}

CFailureDetector::~CFailureDetector(void)
{
	// close(dev);
	
	// panic("[PIR] Exiting\n");
}

void CFailureDetector::enable(void)
{	
	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = handler;
	
	sigaction(SIGUSR1, &act, NULL);
}

void CFailureDetector::disable(void)
{
	act.sa_handler = SIG_IGN;

	sigaction(SIGUSR1, &act, NULL);
}

#include "CFailureDetector.h"

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h> 
#include <sys/ioctl.h>
#include "utils.h"

#define IOCTL_PID 1
#define SIGH (SIGUSR2)

CFailureDetector::CFailureDetector(ISR isr)
{
	pid_t pid;
	
	dev = open("/dev/lampf", O_RDWR);
	if(dev < 0)
		panic("[LampF] Device driver not found.\n");
	
	pid = getpid();
	if(ioctl(dev, IOCTL_PID, &pid))
	{
		close(dev);
		panic("[LampF] Failed system call. Closing device driver.\n");
	}
	
	sigemptyset(&act.sa_mask);
	this->handler = isr;
}

CFailureDetector::~CFailureDetector(void)
{
	close(dev);
	panic("[LampF] Exiting\n");
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

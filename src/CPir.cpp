#include "CPir.h"

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h> 
#include <sys/ioctl.h>
#include "utils.h"

#define IOCTL_PID 1
#define SIGH (SIGUSR1)

CPir::CPir(ISR isr)
{
	pid_t pid;
	 
	dev = open("/dev/pir", O_RDWR);
	if(dev < 0)
		panic("[PIR] Device driver not found.\n");
	
	pid = getpid();
	if(ioctl(dev, IOCTL_PID, &pid))
	{
		close(dev);
		panic("[PIR] Failed system call. Closing device driver.\n");
	}
	
	sigemptyset(&act.sa_mask);
	this->handler = isr; 
}

CPir::~CPir()
{
	close(dev);
	panic("[PIR] Exiting\n");
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

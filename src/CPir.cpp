#include "CPir.h"

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h> 
#include <sys/ioctl.h>
#include "utils.h"

#define SIGHIGH 10
#define IOCTL_PID 1

CPir::CPir(isr pirISR)
{
	pid_t pid;
	
	pirDev = open("/dev/pir", O_RDWR);
	if(pirDev < 0)
		panic("[PIR] Device driver not found.\n");
	
	pid = getpid();
	if(ioctl(pirDev, IOCTL_PID, &pid))
	{
		close(pirDev);
		panic("[PIR] Failed system call. Closing device driver.\n");
	}
	
	sigemptyset(&act.sa_mask);

	pirHand = pirISR;
}

CPir::~CPir(void)
{
	close(pirDev);
	
	panic("[PIR] Exiting\n");
}

void CPir::enable(void)
{	
	act.sa_flags = SA_SIGINFO;
	//act.sa_sigaction = sigHigh;
	act.sa_sigaction = pirHand;
	sigaction(SIGUSR1, &act, NULL);
}

void CPir::disable(void)
{
	act.sa_handler = SIG_IGN;

	sigaction(SIGUSR1, &act, NULL);
}

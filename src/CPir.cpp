#include "CPir.h"

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h> 
#include <signal.h>
#include <sys/ioctl.h>
#include "utils.h"

CPir::CPir(void* pirISR)
{
	struct sigaction act;
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
	act.sa_flags = SA_SIGINFO;
	//act.sa_sigaction = sigHigh;
	act.sa_sigaction = pirISR;
	
	sigaction(SIGUSR1, &act, NULL);
}

CPir::~CPir(void)
{
	close(pirDev);
	
	panic("[PIR] Exiting\n");
}

void CPir::enable(void)
{

}

void CPir::disable(void)
{

}

void CPir::sigHigh(int n, siginfo_t *info, void *unused)
{
	int pinValue = info->si_int;
		
	// printf("Button was pressed. Pin value %d.\n", pinValue);
	// printf("Pin value %d\n", pinValue);

	// static int i = 1;
	// if(pinValue == 1)
	{
		// printf("[%3d] Motion detected\n", i++);
		// exit(1);
	}
	printf("Motion detected\n");
}
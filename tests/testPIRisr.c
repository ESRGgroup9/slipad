#include <stdio.h>
//#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h> 
#include <signal.h>
//#include <sys/time.h>
//#include <string.h>
//#include <sys/types.h>
//#include <sys/stat.h>
#include <sys/ioctl.h>

#define SIGHIGH 10
#define IOCTL_PID 1

void sigHigh(int n, siginfo_t *info, void *unused);

int main(int argc, char *argv[])
{
	int pir;
	struct sigaction act;
	pid_t pid;
	
	pir = open("/dev/pir", O_RDWR);
	if(pir < 0)
	{
		printf("Device driver not found.\n");
		return -1;
	}
	else
		printf("PIR Device driver successfully initialized\n");
	
	pid = getpid();
	if(ioctl(pir, IOCTL_PID, &pid))
	{
		printf("Failed system call. Closing device driver.\n");
		close(pir);
		return -1;
	}
	
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = sigHigh;
	// sigaction(SIGHIGH, &act, NULL);
	sigaction(SIGUSR1, &act, NULL);

	printf("Waiting for movement...\n");
	while(1){}

	close(pir);
	
	printf("\nExiting\n");
	return 0;
}

void sigHigh(int n, siginfo_t *info, void *unused)
{
	// int pinValue = info->si_int;
		
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

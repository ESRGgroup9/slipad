#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h> 
#include <signal.h>
#include <sys/time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

#define SIGHIGH 10
#define IOCTL_PID 1

void sigHigh(int n, siginfo_t *info, void *unused);

int main(int argc, char *argv)
{
	int button;
	struct sigaction act;
	pid_t pid;
	
	button = open("/dev/button", O_RDWR);
	if(button < 0)
	{
		printf("Device driver not found.\n");
		return -1;
	}
	else printf("Device driver successfully initialized\n");
	
	pid = getpid();
	if(ioctl(button, IOCTL_PID, &pid))
	{
		printf("Failed system call. Closing device driver.\n");
		close(button);
		return -1;
	}
	
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = sigHigh;
	// sigaction(SIGHIGH, &act, NULL);
	sigaction(SIGUSR1, &act, NULL);

	printf("Waiting for movement...\n");
	while(1){}

	close(button);
	
	printf("\nExiting\n");
	return 0;
}

void sigHigh(int n, siginfo_t *info, void *unused)
{
	int pinValue = info->si_int;
		
	// printf("Button was pressed. Pin value %d.\n", pinValue);
	// printf("Pin value %d\n", pinValue);

	static int i = 1;
	// if(pinValue == 1)
	{
		printf("[%3d] Motion detected\n", i++);
		// exit(1);
	}
}

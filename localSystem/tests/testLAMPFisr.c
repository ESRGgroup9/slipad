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

#define SIGHIGH SIGUSR2
#define IOCTL_PID 1

void sigHigh(int n, siginfo_t *info, void *unused);

int main(int argc, char *argv[])
{
	int lampf;
	struct sigaction act;
	pid_t pid;
	
	lampf = open("/dev/lampf", O_RDWR);
	if(lampf < 0)
	{
		printf("[LampF] Device driver not found.\n");
		return -1;
	}
	else
		printf("[LampF] Device driver successfully initialized\n");
	
	pid = getpid();
	if(ioctl(lampf, IOCTL_PID, &pid))
	{
		printf("[LampF] Failed system call. Closing device driver.\n");
		close(lampf);
		return -1;
	}
	
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = sigHigh;
	sigaction(SIGHIGH, &act, NULL);

	printf("Waiting for lamp failure...\n");
	while(1){}

	close(lampf);
	
	printf("\nExiting\n");
	// return 0;
}

void sigHigh(int n, siginfo_t *info, void *unused)
{
	// int pinValue = info->si_int;
	printf("Lamp failure detected\n");
}

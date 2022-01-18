#include <sys/stat.h> // umask
#include <stdio.h>	// perror
#include <stdlib.h> // pid_t, EXIT_FAILURE
#include <unistd.h> // fork, setsid, getsid,...
#include <syslog.h>

#include "debug.h"
#include "CSensors.h"

int main(int argc, char *argv[])
{
	// CREATE DAEMON ....
	
	/******************************************************
	* Service implementation
	******************************************************/
	CSensors csensors;

	csensors.run();

	DEBUG_MSG("[dSensors] Exit success");
	return 0;
}
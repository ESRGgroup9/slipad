#include <sys/stat.h> // umask
#include <stdio.h>	// perror
#include <stdlib.h> // pid_t, EXIT_FAILURE
#include <unistd.h> // fork, setsid, getsid,...
#include <syslog.h>
#include <sstream>

#include "debug.h"
#include "CSensors.h"

using namespace std;

int main(int argc, char *argv[])
{
	// Create daemon
	pid_t pid, sid;
	
	// create a new process (child)
	pid = fork();
	if (pid < 0)
	{
		// on error exit
		perror("fork");
		exit(EXIT_FAILURE);
	}

	if (pid > 0)
	{
		// client_recv_pid = pid;
		stringstream debug_msg;
		debug_msg << "Creating dSensors (PID "<< pid << ")" << endl;
		DEBUG_MSG(debug_msg.str());

		// exit parent process
		exit(EXIT_SUCCESS);
	}

	// create a new session
	sid = setsid();
	if (sid < 0)
	{
		// on error exit
		perror("setsid");
		exit(EXIT_FAILURE);
	}
	
	// make '/' the root directory
	if (chdir("/") < 0)
	{
		// on error exit
		perror("chdir");
		exit(EXIT_FAILURE);
	}
	
	// reset umask to 0
	// Any permission may be set (rwx)
	umask(0); 
	// close standard input file descriptor
	close(STDIN_FILENO); 
	// close standard output file descriptor
	close(STDOUT_FILENO);
	// close standard error file descriptor
	close(STDERR_FILENO);
	syslog(LOG_INFO, "Daemon created\n");
	
	/******************************************************
	* Service implementation
	******************************************************/
	CSensors csensors;

	csensors.run();

	DEBUG_MSG("[dSensors] Exit success");
	return 0;
}
#include "utils.h"
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <mqueue.h> // mqd_t
#include <errno.h>

// max length of a message queue
#define MAX_MSG_LEN_R     10000

#define MSGQ_NAME "/dsensors"
#define SIG_CMD_SENT SIGUSR1

int main(int argc, char *argv[])
{
	if(argc < 3)
	{
		printf("Usage: %s <pid> <cmd>\n", argv[0]);
		return 1;
	}

	mqd_t msgqSensors = mq_open(MSGQ_NAME, O_RDWR | O_NONBLOCK, NULL);
	if(msgqSensors == (mqd_t)-1)
		panic("In mq_open()");

	// wait for main PID receival
	// int err;
	// char msg[MAX_MSG_LEN_R];
	// do
	// {
	// 	// read message from msgqueue
	// 	err = mq_receive(msgqSensors, msg, MAX_MSG_LEN_R, NULL);
	// 	if(err == -1)
	// 	{
	// 		// get error from errno
	// 		err = errno;

	// 		// error not expected
	// 		if(err != EAGAIN)
	// 			panic("In mq_receive()");
	// 	}
	// 	// while message queue is empty = wait for PID receival
	// } while(err == EAGAIN);

	// mainPID received
	
	// int mainPID = static_cast<int>(atoi(msg));
	int mainPID = static_cast<int>(atoi(argv[1]));
	printf("Received main PID[%d]\n", mainPID);

	printf("Sending[%s]...\n", argv[2]);
	if(mq_send(msgqSensors, argv[2], strlen(argv[2])+1, 1) != 0)
		panic("In mq_send()");

	kill(mainPID, SIG_CMD_SENT);

	return 0;
}
#include <stdio.h>
#include <mqueue.h>	/* mq_* functions */
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

/* name of the POSIX object referencing the queue */
#define MSGQOBJ_NAME    "/dsensors"
/* max length of a message (just for this process) */
#define MAX_MSG_LEN     10000

int main(int argc, char *argv[]) {
    mqd_t msgq_id;
    char msgcontent[MAX_MSG_LEN];
    int msgsz;
    unsigned int sender;
    struct mq_attr msgq_attr;
    
    /* opening an existing queue  --  mq_open() */
    msgq_id = mq_open(MSGQOBJ_NAME, O_RDWR);
    if (msgq_id == (mqd_t)-1) {
        perror("In mq_open()");
        exit(1);
    }
    /* getting the attributes from the queue   --  mq_getattr() */
    mq_getattr(msgq_id, &msgq_attr);
    // printf("Queue \"%s\":\n\t- stores at most %ld messages\n\t- large at most %ld bytes each\n\t- currently holds %ld messages\n",
                                                      // MSGQOBJ_NAME,msgq_attr.mq_maxmsg, msgq_attr.mq_msgsize, msgq_attr.mq_curmsgs);

    printf("%ld messages queued\n", msgq_attr.mq_curmsgs);
    /* getting a message */
    msgsz = mq_receive(msgq_id, msgcontent, MAX_MSG_LEN, &sender);
    if (msgsz == -1) {
        perror("In mq_receive()");
        exit(1);
    }

    printf("Received message (%d bytes) from %d: '%s'\n", msgsz, sender, msgcontent);
    /* closing the queue    --  mq_close() */
    mq_close(msgq_id);


    // using ls /dev/mqueue we can see that the queue still exists

    /* ...and finally unlink it. After unlink message queue is removed from system. */
    if (mq_unlink(MSGQOBJ_NAME) == -1)
	   perror("Removing queue error");
    
    return 0;
}

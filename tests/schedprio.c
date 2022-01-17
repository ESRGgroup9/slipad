#include <stdio.h>
#include <pthread.h>

int main(int argc, char *argv[])
{
	int thread_policy;
	int tprio = 25;

	pthread_attr_t thread_attr;
	struct sched_param thread_param;
	
	// Step 1: initialize attribute object
	pthread_attr_init (&thread_attr);
	pthread_attr_getschedpolicy (&thread_attr, &thread_policy);
	pthread_attr_getschedparam (&thread_attr, &thread_param);
	
	printf ("Default policy is %s, priority is %d\n",
		(thread_policy == SCHED_FIFO ? "FIFO"
		: (thread_policy == SCHED_RR ? "RR"
		: (thread_policy == SCHED_OTHER ? "OTHER"
		: "unknown"))),thread_param.sched_priority);

	int rr_min_priority, rr_max_priority;

	// Step 2: retrieve min and max priority values for scheduling policy
	rr_min_priority = sched_get_priority_min (SCHED_RR);
	rr_max_priority = sched_get_priority_max (SCHED_RR);

	 // Step 3: calculate and/or assign priority value to sched_param structure
	thread_param.sched_priority = tprio; 
	
	printf ("SCHED_RR priority range is %d to %d: using %d\n",rr_min_priority,
		rr_max_priority,thread_param.sched_priority);

	// Step 4: set attribute object with scheduling parameter
	// pthread_attr_setschedparam (&thread_attr, &thread_param);

	// Step 5: set scheduling attributes to be determined by attribute object
	//pthread_attr_setinheritsched (pthread_attr, PTHREAD_EXPLICIT_SCHED);

	// Step 6: set scheduling policy
	// pthread_attr_setschedpolicy (&thread_attr, SCHED_RR);

	return 0;
}
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

int main(int argc, char *argv)
{
	char rec_data;
	int lampf;
	char state;
	
	lampf = open("/dev/lampf", O_RDWR);
	if(lampf < 0)
	{
		printf("Device driver not found.\n");
		return -1;
	}
	else printf("Device driver successfully initialized!\n");
	
	while(1){
		if(read(lampf, &rec_data, 1) == 0){
			if ( rec_data != state)
			{
				state = rec_data;
				printf("lampf: %c\n", rec_data);
			}
		}
	}

	printf("Closing Device driver.\n");
	close(lampf);
	return 0;
}

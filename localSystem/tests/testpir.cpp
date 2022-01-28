#include <iostream>      //printf()
#include <stdlib.h>     //exit()
#include <signal.h>
#include<unistd.h>

#include "CPir.h"

using namespace std;

void pirISR(int n, siginfo_t *info, void *unused);

void Handler(int signo)
{
    //System Exit
    cout << endl << "Handler:Program stop!" << endl;

    exit(0);
}

int main(int argc, char **argv)
{
	CPir pir(pirISR);

    // Exception handling:ctrl + c
    signal(SIGINT, Handler);
	
	cout << "PIR: enabled!" << endl;
    pir.enable();

	sleep(100);

    cout << "PIR: disabled!" << endl;
    pir.disable();

    sleep(100);

    cout << "PIR: enabled!" << endl;
    pir.enable();
    
    while (1);

    return 0; 
}


void pirISR(int n, siginfo_t *info, void *unused)
{
	cout << "Motion Detected" << endl;
}
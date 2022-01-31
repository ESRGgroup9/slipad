#include <iostream>      
#include <stdlib.h>     //exit()
#include <signal.h>
#include <unistd.h>

#include "CFailureDetector.h"

using namespace std;

void lampfISR(int n, siginfo_t *info, void *unused);

void Handler(int signo)
{
    //System Exit
    cout << endl << "Handler:Program stop!" << endl;

    exit(0);
}

int main(int argc, char *argv[])
{
	CFailureDetector lampf(lampfISR);

    // Exception handling:ctrl + c
    signal(SIGINT, Handler);
	
	cout << "Failure Detector: enabled!" << endl;
    lampf.enable();

	sleep(100);

    cout << "Failure Detector: disabled!" << endl;
    lampf.disable();

    sleep(100);

    cout << "Failure Detector: enabled!" << endl;
    lampf.enable();
    
    while (1);

    return 0; 
}


void lampfISR(int n, siginfo_t *info, void *unused)
{
	cout << "Lamp failure detected!" << endl;
}
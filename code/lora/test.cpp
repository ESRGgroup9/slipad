#include <iostream>
#include <string>
#include <cstring>
#include "LoRa.h"
#include <cstdint>
using namespace std;

#define SEND 1
#define RECV 2
#define ECHO 3

int localAddr = 0xBB;
LoRaClass lora(localAddr);

void print_usage(char *argv0);
void recv(void);
void send(int argc, char *argv[]);

int main(int argc, char *argv[])
{
	int op;

	if(argc < 2)
	{
		print_usage(argv[0]);
		return 1;
	}

	if(strcmp(argv[1], "send") == 0)
	{
		if(argc < 4)
		{
			cout << endl << "Bad send usage" << endl;
			print_usage(argv[0]);
			return 1;
		}

		op = SEND;
	}
	else if(strcmp(argv[1], "recv") == 0)
		op = RECV;
	else if(strcmp(argv[1], "echo") == 0)
		op = ECHO;
	else
	{
		print_usage(argv[0]);
		return 1;
	}

	//-----------------------------------
	cout << "LoRa local address[" << localAddr << "]" << endl;	
	
	// set Lora pins
	// NSS, RST, DIO0
	lora.setup(22, 4, 17);
	
	// set frequency and run
	lora.begin(433E6);

	//-----------------------------------
	// do while op == ECHO
	do
	{
		// RECV or ECHO operation
		if(op != SEND)
			recv();
		// SEND or ECHO operation
		else if(op != RECV)
			send(argc, argv);
	} while(op == ECHO);
	
	return 0;
}

void print_usage(char *argv0)
{
	cout << "Usage: " << endl;
	cout << argv0 << "recv" << endl;
	cout << argv0 << "(send | echo) <recvAddr> <msg>" << endl;
}

void send(int argc, char *argv[])
{
	// cout << endl << "---- LORA SEND ----" << endl;
	LoRaMsg loraMsg;

	// get parameters from argv
	int destAddr = atoi(argv[1]);
	string msg = "";

	for(int i = 3; i < argc; i++)
		msg = msg + argv[i] + " ";

	cout << "Transmitting [" << msg << "] to [" << destAddr << "]...\n";
	loraMsg = lora.sendTo(msg, destAddr);
	cout << endl << loraMsg << endl;
}

void recv(void)
{
	// cout << endl << "---- LORA RECV ----" << endl;
	LoRaMsg loraMsg;		
	LoRaError err = LoRaError::ENOMSGR;

	cout << "Receiving..." << endl;
	do
	{
		err = lora.receive(loraMsg);
	}
	while(err == LoRaError::ENOMSGR);

	cout << "Received message with error[" << static_cast<int>(err) << "]" << endl;
	cout << endl <<  loraMsg << endl;
}
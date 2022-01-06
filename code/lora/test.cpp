#include <iostream>
#include <string>
#include <cstring>
#include "LoRa.h"
#include <cstdint>
using namespace std;

#define SEND 1
#define RECV 2
#define ECHO 3

uint8_t localAddr = 0xBB;
LoRaClass lora(localAddr);

void print_usage(char *argv0);
void recv(void);
void send(int argc, char *argv[]);

#include <bcm2835.h>
#define LORA_SS_PIN        RPI_GPIO_P1_24
#define LORA_RESET_PIN     RPI_GPIO_P1_22
#define LORA_DIO0_PIN      RPI_GPIO_P1_18

int main(int argc, char *argv[])
{
	int op;

// {
// 	// set frequency and run
// 	lora.begin(433E6);
// 	// bcm2835_spi_begin();
// 	cout << "SEND H..." << endl;
// 	lora.testspi('H');

// 	// cout << "SEND E..." << endl;
// 	// lora.testspi('e');

// 	// cout << "SEND L..." << endl;
// 	// lora.testspi('l');

// 	return 0;
// }

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
	{
		if(argc < 4)
		{
			cout << endl << "Bad echo usage" << endl;
			print_usage(argv[0]);
			return 1;
		}
		op = ECHO;
	}
	else
	{
		print_usage(argv[0]);
		return 1;
	}

	//-----------------------------------
	cout << "LoRa local address[" << static_cast<int>(localAddr) << "]" << endl;	
	
	// set Lora pins
	// NSS, RST, DIO0
	lora.setPins(LORA_SS_PIN, LORA_RESET_PIN, LORA_DIO0_PIN);
	
	// set frequency and run
	lora.begin(433E6);
	lora.printFIFORegs();
	//-----------------------------------
	// do while op == ECHO
	do
	{
		// SEND or ECHO operation
		if(op != RECV)
			send(argc, argv);

		// RECV or ECHO operation
		if(op != SEND)
			recv();
	} while(op == ECHO);
	
	lora.printFIFORegs();
	return 0;
}

void print_usage(char *argv0)
{
	cout << "Usage: " << endl;
	cout << argv0 << " recv" << endl;
	cout << argv0 << " (send | echo) <recvAddr> <msg>" << endl;
}

void send(int argc, char *argv[])
{
	// cout << endl << "---- LORA SEND ----" << endl;
	LoRaMsg loraMsg;

	// get parameters from argv
	int destAddr = atoi(argv[2]);
	string msg = "";

	for(int i = 3; i < argc; i++)
		msg = msg + argv[i] + " ";

	// remove last " "
	msg.pop_back();

	cout << "Transmitting [" << msg << "] to [" << destAddr << "]...\n";
	loraMsg = lora.sendTo(msg, destAddr);
	cout << "Transmitted:";
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
#include <iostream>
#include <string>
#include <cstring>
#include "LoRa.h"
#include <cstdint>
using namespace std;

LoRaClass lora;
int destination = 0;
int localAddress = 0;

int main(int argc, char *argv[])
{
	
	// set Lora pins
	// using CS0 as default
	cout << "Setup Lora Pins\n";
	lora.setup(22, 4, 17);
	
	// set frequency and run
	cout << "Set frequency\n";
	lora.begin(433E6);

	cout << "Transmitt '" << msg << "'\n";
	send("hello");
	
	return 0;
}

void send(string msg)
{
	lora.beginPacket();
	
	lora.write(destination);
	lora.write(localAddress);
	// lora.write(msg);
	
	lora.write(msg.length());
	// lora.print(outgoing);	
	lora.write(reinterpret_cast<const uint8_t*>(msg[0]), msg.length());

	lora.endPacket();
}
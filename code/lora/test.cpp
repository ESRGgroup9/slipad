#include <iostream>
#include <string>
#include <cstring>
#include "LoRa.h"
#include <cstdint>
using namespace std;

int main(int argc, char *argv[])
{
	LoRaClass lora(0xBB);	
	
	// set Lora pins
	// using CS0 as default
	cout << "Setup Lora Pins\n";
	lora.setup(22, 4, 17);
	
	// set frequency and run
	cout << "Set frequency\n";
	lora.begin(433E6);

	string msg = "hello";
	cout << "Transmitt '" << msg << "'\n";
	lora.sendTo(msg, 0xFF);
	
	return 0;
}

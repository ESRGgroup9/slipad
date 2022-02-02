#include "CLocalSystem.h"
#include <iostream>
using namespace std;


int main(int argc, char *argv[])
{
	CLocalSystem localsys;
	//CLoraComm lora (433,0xFF, 0xAA);

	// lora.init(1);
	// lora.push("CRQ");
	// lora.run(RUN_NONBLOCK);

	// string msg;

	// while(1)
	// {
	// 	int err = lora.recv(msg);

	// 	if( err == static_cast<int>(LoRaError::MSGOK) )
	// 	{
	// 		cout << "Received: " << msg << endl;
	// 	}
	// 	else if( err != static_cast<int>(LoRaError::ENOMSGR) )
	// 	{
	// 		cout << "Error: " << err << endl;
	// 	}
	// }

	localsys.run();

	cout << endl << "LocalSys: Exited main()" << endl;
	return 0;
}

// #include "CLoraComm.h"
// #include <iostream>
// using namespace std;

// int main(int argc, char *argv[])
// {
//     // CGateway gateway;

//     // gateway.run();

//     CLoraComm lora(433, 0xff, 0xaa);
//     int err;
//     string msg;

//     lora.init(12);
//     lora.send("hello there...");
//     lora.run();

//     while(1)
//     {
//         err = lora.recv(msg);

//         if(err == static_cast<int>(LoRaError::MSGOK))
//         {
//             // get received message
//             // LoRaMsg loraMsg = lora.getMsgAttr();
//             // add LoRa sender address into TCP message payload
//             // msg += PARSE_DELIMITER + loraMsg.sendAddr;
//             // send message
//             cout << "[CGateway::tLoraRecv] Received[" << msg << "]" << endl;
//             // tcp.push(msg);
//         }
//         else if(err != static_cast<int>(LoRaError::ENOMSGR))
//         {
//             cout << "[CGateway::tLoraRecv] Received error [" << err << "]" << endl;
//         }
//     }
// }

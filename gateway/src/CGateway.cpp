#define DEBUG

#include "CGateway.h"
#include "utils.h"
#include "debug.h"
#include "defs.h"

#include <string>
using namespace std;

#define PARSE_DELIMITER ";"

CGateway::CGateway(string host, int port) :
	lora(433, LS_ADDR, GATEWAY_ADDR)
{
	// create LoRa receive from LS
	if(pthread_create(&tLoraRecv_id, NULL, tLoraRecv, this) != 0)
		panic("CGateway::CGateway(): pthread_create");
	
	// init lora threads for sending messages
	lora.init(1);
	// create TCP client
	this->tcphost = host;
	this->tcpport = port; 
	tcp = new CTCPclient(tcphost, tcpport);
}

CGateway::~CGateway()
{

}

void CGateway::connect()
{
	int ret = -1;
	int err;

	stringstream debug_msg;
	debug_msg << "[CGateway::connect] Connecting to " << tcphost << ":" << tcpport << " ..." << endl;
	DEBUG_MSG(debug_msg.str().c_str());

	do
	{
		// connect to tcp server
		ret = tcp->connect();

		if(ret == 0)
		{
			DEBUG_MSG("[CGateway::connect] Connection established");
			break;
		}

		err = errno;
		if(err != ECONNREFUSED)
		{
			// unexpected connection error
			debug_msg.str("");
			debug_msg << "[CGateway::connect] Connect failed: " << string(strerror(err));
			DEBUG_MSG(debug_msg.str().c_str());
		}
	}
	while(err == ECONNREFUSED);
}

#include <bcm2835.h>
void CGateway::run()
{
	// detach from LoRa threads
	lora.run(RUN_NONBLOCK);

	while(1)
	{
		// connect to remote system
		connect();

		// send remote client type to the remote system - Type 0 = GATEWAY
		DEBUG_MSG("[CGateway::run] Identifying as GATEWAY ...");
		
		int ret = -1;
		int err = EAGAIN;

		do
		{
			ret = tcp->send("TYPE;0");
			err = errno;
		}
		while((ret == -1) && (err == EAGAIN));

		bcm2835_delay(2000);

		// create TCP receive thread
		if(pthread_create(&tTCPRecv_id, NULL, tTCPRecv, this) != 0)
			panic("CGateway::CGateway(): pthread_create");

		// start TCP threads
		tcp->init(1);

		// wait for threads termination
		pthread_join(tTCPRecv_id, NULL);

		// run client threads for sending messages
		tcp->run();

		// delete created TCP client and try to reconnect
		tcp->~CTCPclient();

		// Since a socket that had been connected once cannot be reused with
		// another call to connect() one has to create TCPclient dynamically
		// for each connection successfull established
		tcp = new CTCPclient(tcphost, tcpport);
	}
}

void *CGateway::tTCPRecv(void *arg)
{
	// get CGateway instance
	CGateway *c = reinterpret_cast<CGateway*>(arg);
	string msg;
	int ret = -1;

	do
	{
		ret = c->tcp->recv(msg);

		if(ret == -1)
		{
			int err = errno;

			// non blocking operation?
			if(err != EAGAIN)
			{
				// unexpected error
				stringstream debug_msg;
				debug_msg << "ERROR> [CGateway::tTCPRecv] " << string(strerror(err)) << endl;
				ERROR_MSG(debug_msg.str().c_str());
			}
		}
		else if(ret > 0)
		{
			// recv has returned the number of bytes read
			// "parse" receive message. Get message payload and destination addr
			size_t i = msg.find_last_of(PARSE_DELIMITER);
			// set message payload
			string str = msg.substr(0,i);
			// set destination - receiver address
			int destAddr = atoi(msg.substr(i+1).c_str());
			
			c->lora.setDestAddr(destAddr);
			c->lora.push(str);

			stringstream debug_msg;
			debug_msg << "[CGateway::tTCPRecv] Forwarding[" << str << "] to LS[" << destAddr << "]" << endl;
			DEBUG_MSG(debug_msg.str().c_str());
		}
	}
	while(ret != 0);

	DEBUG_MSG("[CGateway::tTCPRecv] RS has closed the connection");
	return NULL;
}

void *CGateway::tLoraRecv(void *arg)
{
	// get CGateway instance
	CGateway *c = reinterpret_cast<CGateway*>(arg);
	string msg;
	LoRaError err = LoRaError::ENOMSGR;
	stringstream debug_msg;

	while(c)
	{
		err = static_cast<LoRaError>(c->lora.recv(msg));

		if(err == LoRaError::MSGOK)
		{
			// get received message
			LoRaMsg loraMsg = c->lora.getMsgAttr();
			// add LoRa sender address into TCP message payload
			msg += PARSE_DELIMITER + to_string(loraMsg.sendAddr);
			// send message
			
			debug_msg.str("");
			debug_msg << "[CGateway::tLoraRecv] Received[" << msg << "]" << endl;
			DEBUG_MSG(debug_msg.str().c_str());
			c->tcp->push(msg);
		}
		else if(err != LoRaError::ENOMSGR)
		{
			cout << "[CGateway::tLoraRecv] Received error [" << static_cast<int>(err) << "]" << endl;
		}
	}

	DEBUG_MSG("[CGateway::tLoraRecv] exiting thread");
	return NULL;
}

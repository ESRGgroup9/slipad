/**
 * @file CRemoteSystem.h
 * @author Tomas Abreu, Diogo Fernandes
 * @date 1 fev 2022
 *
 * @brief Implements the Remote System, defining a TCP server, a list of remote clients
 * of various types (gateway, application, website) and a connection to a database
 */
#ifndef __CREMOTESYSTEM_H__
#define __CREMOTESYSTEM_H__

#include "CTCPserver.h"
#include "CRemoteClient.h"
#include "parser.h"
#include "timer.h"

#include <mysql/mysql.h>
#include <vector>
#include <signal.h>
#include <pthread.h>

class CRemoteSystem
{
public:
/**
 * @brief Start TCP server on given port and connect to database.
 * @param port - port number to listen for new TCP connections
 * @return none
 */
	CRemoteSystem(int port);
	~CRemoteSystem();

/**
 * @brief Listens for new connections and creates new clients when a new connection
 * occurs.
 * @return none 
 *
 * Each time a new connection is done, a thread is created executing tRecvType.
 * This waits for receival of TYPE command, which identifies the type of the 
 * connecting remote client. When the type is defined, the client is added to 
 * the remote client list and the thread terminates.
 */
	void run();

private:
	static void *tRecvType(void *arg);
	static int typeCb(int argc, char *argv[]);

	static void timHandler(union sigval arg);
	void timCheckConnISR();

private:
	static CRemoteSystem *thisPtr;
	Parser typeParser;
	Command_t *cmdList;

private:
	CTCPserver server;
	int client_port;
	std::vector <CRemoteClient*> clientList;
	MYSQL* db;
	Timer timCheckConn;
	pthread_mutex_t mutAddClient;

};

#endif // ! __CREMOTESYSTEM_H__

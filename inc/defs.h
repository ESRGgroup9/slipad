/**
 * @file defs.h
 * @author Tomas Abreu, Diogo Fernandes
 * @date 2 fev 2022
 *
 * @brief Define project definitions used in several modules
 */
#ifndef __DEFS_H__
#define __DEFS_H__

#include <signal.h> // SIGUSR*

/******************************************************************************
 * Local System Definitions
 *****************************************************************************/
// Local system LoRa address
#define LS_ADDR			(uint8_t)(0xaa)
// Message queue name - used to transmit data from dSensors -> CLocalSystem
#define MSGQ_NAME 		"/dsensors"
#define SIG_dSENSORS 	(SIGUSR1)

// Ddrivers interrupt signal number
// must see ddriver definition and classes definitions
#define PIR_SIG_NUM 	(SIGUSR1)
#define LAMPF_SIG_NUM	(SIGUSR2)

/******************************************************************************
 * Remote System Definitions
 *****************************************************************************/
// Database connection details
#define SERVER_PORT 	(5000)
#define HOST			("localhost")
#define USER			("root")
#define PASSWORD		("Password123#@!")
#define DATABASE 		("slipad")

/******************************************************************************
 * Gateway Definitions
 *****************************************************************************/
// Gateway LoRa address
#define GATEWAY_ADDR 	(uint8_t)(0xff)

// Gateway TCP client
#define TCP_HOST		("localhost")
#define TCP_PORT 		(SERVER_PORT)

#endif // !__DEFS_H__

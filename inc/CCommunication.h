/**
 * @file CCommunication.h
 * @author Tomas Abreu, Diogo Fernandes
 * @date 24 jan 2022
 *
 * @brief Class that defines a communication of any kind, defining basic 
 * methods for 'send' and 'recv'.
 */
#ifndef __CCOMMUNICATION_H__
#define __CCOMMUNICATION_H__

#include <pthread.h>
#include <string>
#include <queue>

// Connection status
enum class ConnStatus
{
	CREATED = 0,
	ONLINE,
	ERROR,
	CLOSED
};

// Define CCommunication::run as blocking - waits for threads termination
#define RUN_BLOCK 		(bool)(false)
// Define CCommunication::run as non-blocking - detachs from threads
#define RUN_NONBLOCK	(bool)(true)


class CCommunication
{
public:
/**
 * @brief Initialize all variables and set connection state to ConnStatus::CREATED
 * @return none
 */
	CCommunication();

/**
 * @brief Creates thread for sending messages (tSend)
 * @param tprio - thread priority
 * @return none
 */
	void init(int tprio);

/**
 * @brief Waits for thread termination (default action), or detaches from them.
 * @param async - from asynchronous - defines if this function will be blocking
 * (waiting for thread termination) or not (detaching)
 * @return none
 *
 * Async can be used with defines 'RUN_BLOCK' and 'RUN_NONBLOCK'. If async=RUN_BLOCK
 * the function will join the threads, waiting for their termination. If async=RUN_NONBLOCK
 * the function will detach from the threads.
 */
	void run(bool async = RUN_BLOCK);

/**
 * @brief Get current communication status
 * @return ConnStatus - Communication status
 */
	ConnStatus getStatus(void) const;

/**
 * @brief Adds a new message to the queue of messages to be sent
 * @param msg - string to be added to the queue
 * @return none
 *
 * Adds a message to the queue and signalizes the thread tSent, which is in charge
 * of dispatching the messages from the queue.
 */
	void push(std::string msg);

/**
 * @brief Send a message
 * @param msg - string to be sent
 * @return int - non zero if successfull. -1 if an error occurred
 *
 * Sends a message by calling 'sendFunc' method, which must be defined by
 * derived classes.
 */
	int send(std::string msg);

/**
 * @brief Receive a message
 * @param msg - received string
 * @return int - non zero if successfull. -1 if an error occurred
 *
 * Receives a message by calling 'recvFunc' method, which must be defined by
 * derived classes.
 */
	int recv(std::string &msg);

protected:
/**
 * @brief Thread in charge of dispatching the messages in queue TxMsgs
 * @param arg - must be a pointer to 'this' instance
 * @return null
 *
 * This thread is usually in sleep, being awaken by 'push' when one wants to
 * send a message.
 */
	static void *tSend(void *arg);		
	pthread_cond_t condtSend;
    ConnStatus status;

	// pure virtual methods
	virtual int recvFunc(std::string &msg) = 0;
	virtual int sendFunc(std::string msg) = 0;

private:
	// list of strings to be sent
	std::queue<std::string> TxMsgs;

	pthread_t tSend_id;
	pthread_mutex_t mutComms;
	pthread_mutex_t mutTxMsgs;
};
// End CCommunication class definition

#endif // !__CCOMMUNICATION_H__

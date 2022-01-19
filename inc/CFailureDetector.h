/**
 * @file 	CFailureDetector.cpp, CFailureDetector.h
 * @author 	Diogo Fernandes, Tomás Abreu
 * @date 	18/01/2022
 *
 * @brief	Class that deals with the Lamp Failure sensor  
 */

#ifndef __CFAILUREDETECTOR_H__
#define __CFAILUREDETECTOR_H__

#include <signal.h>

typedef void (*isr)(int, siginfo_t*, void*);

class CFailureDetector
{
public:
	CFailureDetector(isr lampfISR);
	~CFailureDetector(void);

	void enable(void);
	void disable(void);

protected:

private:
	struct sigaction act;
	isr handler;
	int dev;
};

#endif //!__CFAILUREDETECTOR_H__
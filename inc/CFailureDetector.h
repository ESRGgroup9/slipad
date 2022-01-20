/**
 * @file 	CFailureDetector.cpp, CFailureDetector.h
 * @author 	Diogo Fernandes, Tomas Abreu
 * @date 	18/01/2022
 *
 * @brief	Class that deals with the Lamp Failure sensor  
 */

#ifndef __CFAILUREDETECTOR_H__
#define __CFAILUREDETECTOR_H__

#include <signal.h>
#include "interrupt.h"

class CFailureDetector
{
public:
	CFailureDetector(ISR lampfISR);
	~CFailureDetector(void);

	void enable(void);
	void disable(void);

protected:

private:
	struct sigaction act;
	ISR handler;
	int dev;
};

#endif //!__CFAILUREDETECTOR_H__
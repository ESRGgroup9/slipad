/**
 * @file 	CFailureDetector.cpp, CFailureDetector.h
 * @author 	Diogo Fernandes, Tomas Abreu
 * @date 	18/01/2022
 *
 * @brief	Class that deals with the Lamp Failure sensor  
 */

#ifndef __CFAILUREDETECTOR_H__
#define __CFAILUREDETECTOR_H__

#include "CCharacterDev.h"

#include <signal.h>
#include <string>
#include "interrupt.h"

class CFailureDetector : public CCharacterDev
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
};

#endif //!__CFAILUREDETECTOR_H__
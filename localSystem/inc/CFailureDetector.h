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

#include "interrupt.h"
#include <signal.h>

class CFailureDetector : public CCharacterDev
{
public:
	CFailureDetector(void);
	~CFailureDetector(void);

	virtual void enable(void);
	virtual void disable(void);

	char read(void);
	
};

#endif //!__CFAILUREDETECTOR_H__
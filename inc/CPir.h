/**
 * @file 	CPir.cpp, CPir.h
 * @author 	Diogo Fernandes, Tomas Abreu
 * @date 	18/01/2022
 *
 * @brief	Class that deals with the PIR sensor (movement)
 */

#ifndef __CPIR_H__
#define __CPIR_H__

#include "CCharacterDev.h"

#include "interrupt.h" // ISR
#include <signal.h> // sigaction

class CPir : public CCharacterDev
{
public:
	CPir(ISR isr);
	~CPir();
	
	virtual void enable(void);
	virtual void disable(void);

private:
	struct sigaction act;
	ISR handler;
};

#endif //!__CPIR_H__

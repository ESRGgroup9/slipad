/**
 * @file 	CPir.cpp, CPir.h
 * @author 	Diogo Fernandes, Tomás Abreu
 * @date 	18/01/2022
 *
 * @brief	Class that deals with the PIR sensor (movement)
 */

#ifndef __CPIR_H__
#define __CPIR_H__

#include <signal.h>
#include "interrupt.h"

// CPir class
class CPir
{
public:
	CPir(ISR isr);
	~CPir();
	
	void enable(void);
	void disable(void);

private:
	struct sigaction act;
	int dev;

	ISR handler;
};

#endif //!__CPIR_H__

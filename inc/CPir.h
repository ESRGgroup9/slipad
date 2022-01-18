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

typedef void (*isr)(int, siginfo_t*, void*);

class CPir
{
public:
	CPir(isr pirISR);
	~CPir(void);

	void enable(void);
	void disable(void);

private:
	struct sigaction act;
	isr pirHand;
	int pirDev;
};

#endif //!__CPIR_H__


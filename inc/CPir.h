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


class Interrupt
{
public:
    Interrupt(void){};
    // static void Register(int interrupt_numberber, Interrupt* intThisPtr);
    // // wrapper functions to ISR()
    // static void Interrupt_0(void);
    // static void Interrupt_1(void);
    // static void Interrupt_2(void);
    /* etc.*/
    virtual void ISR(void) = 0;

private:
    // static Interrupt* ISRVectorTable[MAX_INTERRUPTS];
};

// forward declaration
class CPir;

// CPir interrupt class
class CPirInterrupt : public Interrupt
{
public:
	CPirInterrupt(CPir* ownerptr);
	virtual void ISR(void);

private:
	CPir *InterruptOwnerPtr;
};

class CPir
{
public:
	CPir(isr pirISR);
	~CPir(void);

	void enable(void);
	void disable(void);

private:
	CPir *InterruptPtr;
	struct sigaction act;
	isr handler;
	int dev;
};

#endif //!__CPIR_H__

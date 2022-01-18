/**
 * @file 	CPir.cpp, CPir.h
 * @author 	Diogo Fernandes, Tomás Abreu
 * @date 	18/01/2022
 *
 * @brief	Class that deals with the PIR sensor (movement)
 */

#ifndef __CPIR_H__
#define __CPIR_H__

class CPir
{
public:
	CPir(void* pirISR(void*));
	~CPir(void);

	void enable(void);
	void disable(void);

private:
	void* isr;
};

#endif //!__CPIR_H__


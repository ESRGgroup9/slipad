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
	CPir(void* pirISR);
	~CPir(void);

	void enable(void);
	void disable(void);
protected:

private:
	void sigHigh(int n, siginfo_t *info, void *unused);
	int pirDev;
}; 

#endif //!__CPIR_H__


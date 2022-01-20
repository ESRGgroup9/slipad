#ifndef __DEBUG_H__
#define __DEBUG_H__

void ERROR_MSG(char *msg);

#ifdef DEBUG
// **********************************************
#include <iomanip>
#include <stdarg.h>
#include <iostream>

#define DEBUG_MSG(str) (std::cout << str << std::endl)
#define ERROR_MSG(str)	{DEBUG_MSG(str); abort();}

// **********************************************
#else // DEBUG not defined

#define DEBUG_MSG(str)
#define ERROR_MSG(str)

#endif // !DEBUG
#endif // !__DEBUG_H__

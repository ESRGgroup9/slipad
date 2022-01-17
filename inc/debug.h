#ifndef __DEBUG_H__
#define __DEBUG_H__

// **********************************************
#ifdef DEBUG

#include <iomanip>
#include <stdarg.h>
#include <iostream>

// uint8_t tprintf(const char *format, ...);
#define DEBUG_MSG1(...) printf(__VA_ARGS__)

#define DEBUG_MSG(str) (std::cout << str << std::endl);
// template <typename ... Args>
// void DEBUG_MSG(Args ... args);

// #define DEBUG_MSG(args...) if (DEBUG) _debug(args);

// **********************************************
#else // DEBUG not defined

#define DEBUG_MSG(str)

#endif // !DEBUG
#endif // !__DEBUG_H__

#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>

void panic(char *msg);
#define panic(m)	{perror(m); abort();}

// #define DEBUG

#ifdef DEBUG
  #include <iomanip>
  #include <fstream>

  ofstream logfile("log.txt");
  #define DEBUG_MSG(str) (logfile << str << std::endl)

#else
  #define DEBUG_MSG(str) 
#endif

#endif // !__UTILS_H__

#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h> // perror
#include <stdlib.h> // abort

void panic(char *msg);
#define panic(m)	{perror(m); abort();}

#endif // !__UTILS_H__

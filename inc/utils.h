#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>

void panic(char *msg);
#define panic(m)	{perror(m); abort();}

// void create_timer(unsigned i, void *handler);

#endif // !__UTILS_H__

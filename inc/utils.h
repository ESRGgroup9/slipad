#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>

void panic(char *msg);
#define panic(m)	{perror(m); abort();}

// void create_timer(unsigned seconds, void (*handler)(union sigval));

#endif // !__UTILS_H__

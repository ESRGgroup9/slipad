#ifndef __PARSER_H__
#define __PARSER_H__

#include <cstring>
#include <errno.h>

#define ECMDNF  	1 // Command not found
#define ENOCMD  	2 // Command is empty
#define ENOLIST 	3 // List is empty

// #define ENOMEM  	4 // No memory available or bad allocation of memory
// #define EPERM 	5 // Permission error
 
// #define EINVARG 	6 // Invalid (number of) argument(s)
// #define EALREADY 7 // Operation already in progress
// #define ENOP		8 // No operation in progress

// #define ENOKEY	9 // Required parameter/key not defined

typedef int (*Command_cb)(int, char *[]);

// brief simple struct to hold data for a single command
struct Command_t
{
	const char *cmd;	// the command string to match against
	Command_cb fn;		// the function to call when cmd is matched
};

class Parser
{
public:
	Parser(Command_t *cmd_list, const char *delimeter);
	int parse(const char *str);

private:
	const char *delimeter;
	Command_t *cmd_list;
};

#endif // !__PARSER_H__

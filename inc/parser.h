#ifndef __PARSER_H__
#define __PARSER_H__

#include <string>

typedef int (*Command_cb)(int, char *[]);

// brief simple struct to hold data for a single command
struct Command_t
{
	const std::string cmd;	// the command string to match against
	Command_cb fn;		// the function to call when cmd is matched
};

class Parser
{
public:
	Parser(Command_t *cmd_list, const char *delimeter);
	int parse(std::string str);

private:
	const char *delimeter;
	Command_t *cmd_list;
};

#endif // !__PARSER_H__

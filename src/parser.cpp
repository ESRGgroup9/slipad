#include "parser.h"

#include <cstdlib>
#include <cstdint>
#include <cstring>

Parser::Parser(Command_t *cmd_list, const char *delimeter) :
	delimeter(delimeter),
	cmd_list(cmd_list)
{

}

int Parser::parse(const char *str_in)
{
	char **argv = NULL; // List of arguments
	char *arg; // aux variable. Holds latest found argument
	char *s; // copy of received command. Necessary to use strtok
	
	uint8_t len;
	uint8_t argc = 0;
	int retval = (-ECMDNF);
	
	if(cmd_list == NULL)
		return (-ENOLIST);
	
	if ((str_in == NULL) || (strlen(str_in) == 0)) // empty command
		return (-ENOCMD);

	len = strlen(str_in) + 1;
	s = (char*)malloc(len);                 // just in case str_in is const
	
	if(s == NULL)
		return (-ENOMEM);
	
	strcpy(s, str_in);
	arg = strtok(s, delimeter);
	while (arg)
	{
		argc++;
		len = sizeof (argv);
		if(argc == 1)
				argv = (char**)malloc(len);
		else
				argv = (char**)realloc(argv, argc * len);

		argv[argc-1] = arg;
		arg = strtok(NULL, delimeter);
	}

	if (argc)
	{
		const Command_t *p;

		for(p = cmd_list; p->cmd; p++)
		{
			if(strcmp(p->cmd, argv[0]) == 0)
			{
				retval = (p->fn(argc, argv)); // Execute func
				break;
			}
		}
	}

	// command not found
	if(argv)
		free(argv);

	free(s);

	return retval;
}
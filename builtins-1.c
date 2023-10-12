#include "main.h"

/**
 * env - exits the program
 * @args: array of argument tokens
 *
 * Return: exit status
 */
int env_fn(exec_info ei)
{
	size_t i;

	(void)ei;

	for (i = 0; environ[i]; i++)
		printf("%s\n", environ[i]);
	return (0);
}

/**
 * exit - exits the program
 * @args: array of argument tokens
 *
 * Return: exit status
 */
int exit_fn(exec_info ei)
{
	if (ei.argc > 1)
	{
		if (is_digits(ei.argv[1]))
			last_exit_code = atoi(ei.argv[1]);
		else
		{
			dprintf(
			    2, "%s: %d: %s: Illegal number: %s\n",
			    ei.shell_argv[0], ei.loopcnt, ei.cmd_name,
			    ei.argv[1]);
			return (EXIT_ILLEGAL_NUM);
		}
	}
	return (EXIT_IMMEDIATE);
}

int is_digits(char *str)
{
	int i = 0;

	while (str[i])
		if (!isdigit(str[i++]))
			return (0);
	return (str[0] ? 1 : 0);
}

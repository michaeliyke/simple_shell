#include "main.h"

/**
 * env - exits the program
 * @args: array of argument tokens
 *
 * Return: exit status
 */
int env_fn(char **args)
{
	size_t i;

	(void)args;

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
int exit_fn(char **args)
{
	(void)args;
	return (EXIT_IMMEDIATE);
}

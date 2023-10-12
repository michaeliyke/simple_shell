#include "main.h"

/**
 * cd - changes working directory
 * @args: array of argument tokens
 *
 * Return: exit status
 */
int cd_fn(char **args)
{
	char *path;
	char *cwd = malloc(sizeof(char) * 30);
	int status;

	getcwd(cwd, 30);

	if (*(args + 1) == NULL)
		path = _getenv("HOME");
	else if (*(args + 1)[0] == '-')
		printf("%s\n", path = _getenv("OLDPWD"));
	else if (*(args + 1)[0] == '/')
		path = args[1];
	else
		if (path_exists(args[1]))
			path = realpath(args[1], NULL);

	if (path == NULL)
		return (0);
	status = chdir(path);
	if (status == 0)
	{
		_setenv("OLDPWD", _getenv("PWD"), 1);
       		_setenv("PWD", path, 1);
	}
	else
	{
		dprintf(2, "./hsh: 1: cd: can't cd to %s\n", args[1]);
	}
	return (0);
}
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

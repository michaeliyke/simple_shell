#include "main.h"

/**
 * cd_fn - changes working directory
 * @ei: execution info
 *
 * Return: exit status
 */
int cd_fn(exec_info *ei)
{
	char *path;
	char *cwd = malloc(sizeof(char) * 30);
	int status;

	getcwd(cwd, 30);

	if (*(ei->argv + 1) == NULL)
		path = _getenv("HOME");
	else if (*(ei->argv + 1)[0] == '-')
		printf("%s\n", path = _getenv("OLDPWD"));
	else if (*(ei->argv + 1)[0] == '/')
		path = ei->argv[1];
	else if (path_exists(ei->argv[1]))
		path = realpath(ei->argv[1], NULL);

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
		dprintf(2, "./hsh: 1: cd: can't cd to %s\n", ei->argv[1]);
	}
	return (0);
}

/**
 * env_fn - exits the program
 * @ei: execution information
 *
 * Return: exit status
 */
int env_fn(exec_info *ei)
{
	size_t i;

	(void)ei;

	for (i = 0; environ[i]; i++)
		printf("%s\n", environ[i]);
	return (0);
}

/**
 * exit_fn - exits the program
 * @ei: execution information
 *
 * Return: exit status
 */
int exit_fn(exec_info *ei)
{
	if (ei->argc > 1)
	{
		if (is_digits(ei->argv[1]))
			ei->last_exit_code = atoi(ei->argv[1]);
		else
		{
			dprintf(
			    2, "%s: %d: %s: Illegal number: %s\n",
			    ei->shell_argv[0], ei->loopcnt, ei->cmd_name,
			    ei->argv[1]);
			return (EXIT_ILLEGAL_NUM);
		}
	}
	return (EXIT_IMMEDIATE);
}

/**
 * setenv_fn - sets a variable in the user environment
 * @ei: runtime execution information
 *
 * Return: 0 if successful and -1 if not
 */
int setenv_fn(exec_info *ei)
{
	char **argv = ei->argv;
	int argc = ei->argc;

	/* TODO:  should print sth to stderr on failure*/

	if (argc < 3)
	{
		dprintf(2, "Failure");
		return (-1);
	}
	if (_setenv(argv[1], argv[2], 1) == 0)
		return (0);
	dprintf(2, "Failure");
	return (-1);
}

/**
 * unsetenv_fn - sets a variable in the user environment
 * @ei: runtime execution information
 *
 * Return: 0 if successful and -1 if not
 */
int unsetenv_fn(exec_info *ei)
{
	char **argv = ei->argv;
	int argc = ei->argc;

	/* TODO:  should print sth to stderr on failure*/

	if (argc < 2)
	{
		dprintf(2, "Failure");
		return (-1);
	}
	if (_unsetenv(argv[1]) == 0)
		return (0);
	dprintf(2, "Failure");
	return (-1);
}

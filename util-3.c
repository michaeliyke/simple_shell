#include "main.h"

/**
 * executor - the middle multiplexer
 * @ei: the execution information for effective reference
 *
 * Return: -1 on error or the exit code of the program just executed
 */
int executor(exec_info *ei)
{
	/* Handle builtin calls */
	builtInHandler fn = builtin_handler(ei->cmd_name);
	int status = 0;
	char *tmp;

	if (fn)
		/* it is a built in */
		return (fn(ei));

	/* Handle external commands */
	tmp = extern_handler(ei->cmd_name);
	ei->argv[0] = tmp ? tmp : ei->argv[0]; /* Don't overwrite right away */
	if (tmp == NULL)
	{

		dprintf(
		    2, "%s: %d: %s: not found\n",
		    ei->shell_argv[0], ei->loopcnt, ei->cmd_name);
		return (EXIT_NOT_FOUND);
	}
	else
		/* Handle external command here */
		return (exec_child(ei));
	return (status);
}

/**
 * exec_child - execute command in child process and return the exit code
 * @ei: the execution information
 *
 * Return: exit code of the program
 */
int exec_child(exec_info *ei)
{
	pid_t child;
	int status = 0;

	child = fork();
	if (child == 0)
	{
		if (execve(ei->argv[0], ei->argv, ei->env) == -1)
		{ /* exit status of -1 means command not found */
			dprintf(
			    2, "%s: %d: %s: not found\n",
			    ei->shell_argv[0], ei->loopcnt, ei->cmd_name);
			_exit(EXIT_NOT_FOUND);
		} /* This has not effect as child already exited */
	}
	else
	{ /* Await the exit status of the child, not program */

		wait(&status);
		if (WIFEXITED(status))
		{ /* Program shutdown by itself and was not forced */
			return (WEXITSTATUS(status));
		} /* The exit code of the program just ran */
	}
	return (0);
}

/**
 * builtin_handler - gets the builtin handler of a builtin command
 * @command_name: the name of command
 *
 * Return: pointer to the handler function of the builtin
 */
builtInHandler builtin_handler(char *command_name)
{
	builtin builtins[] = {
	    {"env", NULL, env_fn},
	    {"exit", NULL, exit_fn},
	    {"cd", NULL, cd_fn},
	    {"setenv", NULL, setenv_fn},
	    {"unsetenv", NULL, unsetenv_fn},
	    {NULL, NULL, NULL},
	};
	int i;

	for (i = 0; i < NUM_BUILTINS; i++)
	{
		if (strcmp(builtins[i].name, command_name) == 0)
			return (builtins[i].handler);
	}
	return (NULL);
}

/**
 * extern_handler - gets the full path of an external command or NULL
 * @command_name: the name of command
 *
 * Return: pointer to the full path of external program or NULL
 */
char *extern_handler(char *command_name)
{
	char *command_path;
	int exists_s = exists(command_name, &command_path);

	return (exists_s ? command_path : NULL);
}

/**
 * get_toks - collects the tokens of getline
 * @s: the string
 *
 * Return: array of string tokens
 */
char **get_toks(char *s)
{
	int n = 0, wc = word_count(s);
	char **argv, *buff, *temp;

	if (s == NULL || *s == '\0')
		return (NULL);
	argv = malloc(sizeof(char *) * (wc + 1));
	temp = strdup(s);
	buff = _strtok(temp, " \n");
	for (; buff; n++)
	{

		argv[n] = strdup(buff);
		buff = _strtok(NULL, " \n");
	}
	argv[n] = NULL;
	free(temp);
	return (argv); /* should be free'd when done */
}

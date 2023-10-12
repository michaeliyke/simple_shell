#include "main.h"

/**
 * executor - the middle multiplexer
 * @command_name: name of command
 * @argv: array of argument tokens strings from getline call
 * @av: original argv parameter passed from the commandline during call
 * @env: the environment variables list
 * @loopcnt: loop count from main to keep track of loop count
 *
 * Return: -1 on error or the exit code of the program just executed
 */
int executor(char *cmd_name, char **av, char **argv, char **env, int loopcnt)
{
	/* Handle builtin calls */
	builtInHandler fn = builtin_handler(cmd_name);
	pid_t child;
	int status = 0;
	char *tmp;

	if (fn)
	{
		/* it is a built in */
		return (fn(argv));
	}

	/* Handle external commands */
	tmp = extern_handler(cmd_name);
	argv[0] = tmp ? tmp : argv[0]; /* Don't overwrite argv[0] right away */
	if (tmp == NULL)
	{

		dprintf(
		    2, "%s: %d: %s: not found\n", av[0], loopcnt, cmd_name);
		return (EXIT_NOT_FOUND);
	}
	else
	{
		/* Handle external command here */
		child = fork();
		if (child == 0)
		{
			if (execve(argv[0], argv, env) == -1)
			{ /* exit status of -1 means command not found */
				dprintf(
				    2, "%s: %d: %s: not found\n", av[0],
				    loopcnt, cmd_name);
				_exit(EXIT_NOT_FOUND); /* This has not effect as child already exited */
			}
		}
		else
		{
			wait(&status);	       /* Awaits the exits status of child process, not the program */
			if (WIFEXITED(status)) /* Program shutdown by itself and not forced by system */
			{
				return (WEXITSTATUS(status)); /* The exit code of the program just ran */
			}
		}
		return (status);
	}
	return (status);
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
	char **argv = malloc(sizeof(char *) * 31), *buff;
	int n;

	buff = _strtok(s, " \n");
	for (n = 0; buff; n++)
	{

		argv[n] = buff;
		buff = _strtok(NULL, " \n");
	}
	argv[n] = NULL;
	return (argv);
}

/**
 * free_str_arr - free an arry of strings
 * @arr: the array to free
 * @limit: integer limit
 *
 * Return: void
 */
void free_str_arr(char **arr, int limit)
{
	int i = 0;

	while (i < limit)
		free(arr[i++]);
	free(arr);
}

/**
 * exit_or_cont - The program will exit here using exit code
 * @exit_code: exit status code
 *
 * Return: the exit code of last command
 */
int exit_or_cont(int exit_code)
{
	static int last_exit_code = INT_MAX; /* Default value for first run */

	switch (exit_code)
	{
	case EXIT_NOT_FOUND:
		exit(EXIT_NOT_FOUND);
		break;
	case EXIT_IMMEDIATE:
		/**
		 * This comes from the exit inbuilt
		 * The default is 0 meaning no command has been ran yet
		 */
		exit(last_exit_code == INT_MAX ? 0 : last_exit_code);
		break;
	}
	last_exit_code = exit_code; /* save the exit code for future use */
	return (last_exit_code);
}

#include "main.h"

/**
 * executor - the middle multiplexer
 * @command_name: name of command
 * @tokenized_args: array of argument tokens strings
 *
 * Return: void
 */
int executor(char *cmd_name, char **av, char **argv, char **env, int loopcnt)
{
	/* Handle builtin calls */
	builtInHandler fn = builtin_handler(cmd_name);
	pid_t child;
	int status;

	if (fn)
	{
		/* it is a built in */
		printf("Builtin %s FOUND!\n", cmd_name);
		return (0);
	}

	/* Handle external commands */
	argv[0] = extern_handler(cmd_name);
	if (argv[0] == NULL)
	{

		dprintf(
		    2, "%s: %d: %s: not found\n", av[0], loopcnt, cmd_name);
		exit(127);
	}
	else
	{
		/* Handle external command here */
		child = fork();
		if (child == 0)
		{
			if (execve(argv[0], argv, env) == -1)
				perror("Execution failed"),
				    _exit(EXIT_FAILURE);
		}
		else
			wait(&status);
		return (0);
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
	    {"ls_", NULL, ls_fn},
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

	buff = strtok(s, " \n");
	for (n = 0; buff; n++)
	{

		argv[n] = buff;
		buff = strtok(NULL, " \n");
	}
	argv[n] = NULL;
	return (argv);
}
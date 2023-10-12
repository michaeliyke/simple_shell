#include "main.h"

/**
 * executor - the middle multiplexer
 * @command_name: name of command
 * @argv: argv for the command being eexcuted (getline call)
 * @av: original argv parameter passed from simple_shell commandline call
 * @env: the environment variables list
 * @lc: loop count from main to keep track of loop count
 * @ac: argc for the command being executed
 *
 * Return: -1 on error or the exit code of the program just executed
 */
int executor(exec_info ei)
{
	/* Handle builtin calls */
	builtInHandler fn = builtin_handler(ei.cmd_name);
	pid_t child;
	int status = 0;
	char *tmp;

	if (fn)
	{
		/* it is a built in */
		return (fn(ei));
	}

	/* Handle external commands */
	tmp = extern_handler(ei.cmd_name);
	ei.argv[0] = tmp ? tmp : ei.argv[0]; /* Don't overwrite argv[0] right away */
	if (tmp == NULL)
	{

		dprintf(
		    2, "%s: %d: %s: not found\n",
		    ei.shell_argv[0], ei.loopcnt, ei.cmd_name);
		return (EXIT_NOT_FOUND);
	}
	else
	{
		/* Handle external command here */
		child = fork();
		if (child == 0)
		{
			if (execve(ei.argv[0], ei.argv, ei.env) == -1)
			{ /* exit status of -1 means command not found */
				dprintf(
				    2, "%s: %d: %s: not found\n",
				    ei.shell_argv[0], ei.loopcnt, ei.cmd_name);
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
	switch (exit_code)
	{
	case EXIT_NOT_FOUND:
		exit(EXIT_NOT_FOUND);
		break;
	case EXIT_ILLEGAL_NUM:
		exit(EXIT_ILLEGAL_NUM);
		break;
	case EXIT_IMMEDIATE:
		/**
		 * This comes from the exit inbuilt
		 * The default is 0 meaning no command has been ran yet
		 * (last_exit_code == INT_MAX ? 0 : last_exit_code)
		 */
		exit((last_exit_code == INT_MAX ? 0 : last_exit_code));
		break;
	}
	last_exit_code = exit_code; /* save the exit code for future use */
	return (last_exit_code);
}

/**
 * word_count - count the number of words in a string
 * @str: string of characters
 *
 * Return: count of words in a str
 *
 * NOTE: Words are delimitred by spaces and newlines only
 */
int word_count(char *str)
{
	int in_word = 0, count = 0;

	if (!str)
		return (0);
	while (*str)
	{
		if (isspace(*str)) /* if space, set in_word, and skip over */
			in_word = 0;
		else /* if not space */
		{    /** if we are in word, skip until we find a space
		      * if we find a space, update in_word and count
		      * */
			if (!in_word)
				in_word = 1, count++;
		}
		str++;
	}
	return (count);
}

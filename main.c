#include "main.h"

/**
 * main - main shell func
 * @ac: arg count
 * @av: arg vector
 * @env: environment
 *
 * Return: 0
 */
int main(int ac, char **av, char **env)
{
	char **lines, *user_input = NULL;
	int loopcnt = 0, term = isatty(0);
	size_t buff_size = 0, x; /* x is a counter var */
	ssize_t read_status;
	exec_info info;
	alias_t *al_list = NULL;
	FILE *f = ac == 2 ? fopen(av[1], "r") : stdin;
	int lc; /* Number of commands found in user input */

	info.last_exit_code = INT_MAX; /* Default value for first run */
	info.al_list = al_list;

	while (++loopcnt)
	{
		if (f == NULL)
		{
			dprintf(2, "%s: 0: Can't open %s\n", av[0], av[1]);
			exit(127);
		}
		if (term == 1 && ac != 2)
			printf("$ ");
		read_status = get_line(&user_input, &buff_size, f);
		if (read_status == -1)
			break;
		lines = get_cmd_lines(user_input, &lc); /* all the cmd lines */
		if (lines == NULL)
			free(user_input);
		info.queued = lc; /* How many commands in queue */
		info.LINES = lines, info.USER_INPUT = user_input;
		for (x = 0; lines[x] != NULL; x++)
		{ /* lines[x] reps each user command assuming multiple */
			run(av, env, &info, lines[x], loopcnt);
		}
	}
	if (ac == 2)
		fclose(f);
	free_list(info.al_list);
	return (0);
}

/**
 * run_bool - runs a given line conditionally.
 * Assumes multiple commands were given along with boolean conditions
 * @av: The argv argument from main func
 * @env: the env argument from main func
 * @ei: pointer to exec_info object passing to command handlers
 * @ptr: pointer to the current command being executed from main func
 * @lc: the loop count from main func
 *
 * Return: the status code of last command executed
 */
int run_bool(char **av, char **env, exec_info *ei, char *ptr, int lc)
{
	char **toks, *cmds = strdup(ptr), *cmds_ = cmds; /* preserves *ptr */
	int status_code = INT_MAX, status;
	char *cmd = trim(get_next_boundary(ei, &cmds));

	(void)lc;
	(void)av;
	(void)env;
	while (cmd)
	{			      /* Tobe free'd: cmds, toks */
		toks = get_toks(cmd); /* free with free_str_arr */
		ei->cmd_name = toks[0];
		ei->argv = toks;
		ei->argc = word_count(cmd);

		if (status_code == INT_MAX)
			status_code = executor(ei);
		else if (status_code != 0 && bool_is(ei, "||"))
			status_code = executor(ei);
		else if (status_code == 0 && bool_is(ei, "&&"))
			status_code = executor(ei);
		status = status_code;

		free(cmd);
		cmd = trim(get_next_boundary(ei, &cmds));
		/* Below ensures that the mems being created are free'd */
		if (cmd)
			free(toks);
	}
	free(cmds_); /* toks to be free'd in exit_or_cont() */
	return (status);
}

/**
 * run - runs a given line of command.
 * Assuming multiple commands were given, run executes each at a time
 * @av: The argv argument from main func
 * @env: the env argument from main func
 * @ei: pointer to exec_info object passing to command handlers
 * @ptr: pointer to the current command being executed from main func
 * @loopcnt: the loop count from main func
 *
 * Return: void
 */
void run(char **av, char **env, exec_info *ei, char *ptr, int loopcnt)
{
	int exit_s = 0;
	int word_cnt = word_count(ptr); /* The argc for the cmd */
	char **toks;
	alias_t *n;

	(void)n;
	if (word_cnt < 1)
		return;
	toks = get_toks(ptr);	/* Token of a given command */
	ei->cmd_name = toks[0]; /* Command name or path */
	for (n = ei->al_list; n != NULL; n = n->next)
	{
		if (strcmp(toks[0], n->name) == 0)
			ei->cmd_name = n->value;
	}
	ei->argv = toks;       /* Our command's argv */
	ei->shell_argv = av;   /* Argv passed to main func */
	ei->argc = word_cnt;   /* Our command's argc */
	ei->env = env;	       /* The env passed to main func*/
	ei->loopcnt = loopcnt; /* Loop count in main func */
	if (check_has_bool(ptr))
	{ /* Above checks if command has boolean operators */
		exit_s = check_unexpected(ptr, ei);
		free_str_arr(toks); /* Free current call to get_toks() */
		if (exit_s == 0)    /* unexpected syntax was not found */
			exit_s = run_bool(av, env, ei, ptr, loopcnt);
		/* else unexpected syntax was found, go straight to warning */
	}
	else
		exit_s = executor(ei); /* Normal simple execution */

	/* toks var s cleaned up within exit_or_cont() */
	exit_or_cont(exit_s, ei); /* Using status code to quit/not */
}

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
	char **lines, *user_input = NULL, **ptr;
	int loopcnt = 0, term = isatty(0);
	size_t buff_size = 0;
	ssize_t read_status;
	exec_info info;
	alias_t *al_list = NULL;
	int lc; /* Number of commands found in user input */

	(void)ac;
	info.last_exit_code = INT_MAX; /* Default value for first run */
	info.al_list = al_list;
	while (++loopcnt)
	{
		if (term == 1)
			printf("$ ");
		read_status = get_line(&user_input, &buff_size, stdin);
		if (read_status == -1)
			break;
		lines = get_cmd_lines(user_input, &lc); /* all the cmd lines */
		if (lines == NULL)
			free(user_input);
		info.queued = lc; /* How many commands in queue */
		for (ptr = lines; ptr && *ptr != NULL; ptr++)
		{
			run(av, env, &info, ptr, loopcnt);
		}
		free_str_arr(lines); /* user_input was free'd in get_line */
	}
	return (0);
}

/**
 * run - runs a given line of command.
 * Assuming multiple commands were given, run executes each at a time
 * @av: The argv argument from main func
 * @env: the env argument from main func
 * @ei: pointer to exec_info object passing to command handlers
 * @ptr: pointer to the current command being executed from main func
 * @loopcnt: the loop count from main func
 * @lc: the command lines count that hold the number of command in queue
 *
 * Return: void
 */
void run(char **av, char **env, exec_info *ei, char **ptr, int loopcnt)
{
	int exit_s = 0;
	int word_cnt = word_count(*ptr); /* The argc for the cmd */
	char **toks;
	alias_t *n;

	if (word_cnt > 0) /* exec with the info obj and return a status code */
	{
		toks = get_toks(*ptr);	  /* Token of a given command */
		ei->cmd_name = toks[0];	  /* Command name or path */
		for (n = ei->al_list; n != NULL; n = n->next)
		{
			if (strcmp(toks[0], n->name) == 0)
				ei->cmd_name = n->value;
		}
		ei->argv = toks;	  /* Our command's argv */
		ei->shell_argv = av;	  /* Argv passed to main func */
		ei->argc = word_cnt;	  /* Our command's argc */
		ei->env = env;		  /* The env passed to main func*/
		ei->loopcnt = loopcnt;	  /* Loop count in main func */
		exit_s = executor(ei);	  /* execute and return status code */
		exit_or_cont(exit_s, ei); /* Using status code to quit/not */
		free_str_arr(toks);	  /* Cleanup the memory used by tok */
	}
}

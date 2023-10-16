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
	char **toks, **lines, *user_input = NULL, **ptr;
	int loopcnt = 0, term = isatty(0), exit_s = 0, word_cnt;
	size_t buff_size = 0;
	ssize_t read_status;
	exec_info info;

	(void)ac;
	info.last_exit_code = INT_MAX; /* Default value for first run */
	while (++loopcnt)
	{
		if (term == 1)
			printf("$ ");
		read_status = get_line(&user_input, &buff_size, stdin);
		if (read_status == -1)
		{
			free(user_input); /* to be free'd even upon failure */
			break;
		}
		lines = get_cmd_lines(user_input); /* Get all the cmd lines */
		if (lines == NULL)
			free(user_input);
		for (ptr = lines; ptr && *ptr != NULL; ptr++)
		{
			word_cnt = word_count(*ptr); /* The argc for the cmd */
			if (word_cnt > 0)
			{ /* exec with the info obj and return a status code */
				toks = get_toks(*ptr);
				info.cmd_name = toks[0], info.argv = toks;
				info.shell_argv = av, info.argc = word_cnt;
				info.env = env, info.loopcnt = loopcnt;
				exit_s = executor(&info);
				exit_or_cont(exit_s, &info), free_str_arr(toks);
			}
		}
		free_str_arr(lines); /* user_input was free'd in get_line */
	}
	return (0);
}

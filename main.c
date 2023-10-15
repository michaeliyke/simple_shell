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
	char *line = NULL, **toks;
	int loopcnt = 0, term = isatty(0), exit_s = 0, word_cnt;
	size_t i = 0; /* TODO: i variable to be renamed descriptively */
	ssize_t read_status;
	exec_info info;

	(void)ac;
	(void)exit_s;
	(void)read_status;
	info.last_exit_code = INT_MAX; /* Default value for first run */
	while (1)
	{
		loopcnt++;
		if (term == 1)
			printf("$ ");
		read_status = get_line(&line, &i, stdin);
		if (read_status == -1)
			break;
		word_cnt = word_count(line);
		if (word_cnt < 1)
			continue;
		/* exec a command and return a status code */
		toks = get_toks(line);
		info.cmd_name = toks[0];
		info.argv = toks;
		info.shell_argv = av;
		info.argc = word_cnt;
		info.env = env;
		info.loopcnt = loopcnt;
		exit_s = executor(&info);
		exit_or_cont(exit_s, &info);
		free_get_toks(toks);
	}
	return (0);
}

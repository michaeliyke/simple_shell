#include "main.h"

int last_exit_code = INT_MAX; /* Default value for first run */
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
	char *line = NULL, *buff, *tmp;
	int loopcnt = 0, term = isatty(0), exit_s = 0, word_cnt;
	size_t i = 0; /* TODO: i variable to be renamed descriptively */
	ssize_t read_status;
	exec_info info;

	(void)ac;
	(void)exit_s;
	(void)read_status;
	while (1)
	{
		loopcnt++;
		if (term == 1)
			printf("$ ");
		read_status = get_line(&line, &i, stdin);
		if (read_status == -1)
			break;
		word_cnt = word_count(line);
		tmp = strdup(line);
		buff = strdup(_strtok(tmp, " \n")); /* raw command name */
		if (buff)
		{ /* if (buff), exec the cmd and return status code */
			info.cmd_name = buff;
			info.argv = get_toks(line);
			info.shell_argv = av;
			info.argc = word_cnt;
			info.env = env;
			info.loopcnt = loopcnt;
			exit_s = executor(info);
		}
		free(tmp);
		i = 0;
		/* while (info.argv[i])
		{
			printf("%s\n", info.argv[i]);
			free(info.argv[i]);
			i++;
		}
		free(info.argv); */
		exit_or_cont(exit_s);
	}
	return (0);
}

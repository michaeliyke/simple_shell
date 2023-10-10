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
	char *line = NULL, *buff;
	int loopcnt = 0, term = isatty(0), exit_s = 0;
	size_t i = 0; /* TODO: i variable to be renamed descriptively */
	ssize_t read_status;

	(void)ac;
	(void)av;
	(void)exit_s;
	setenv("TERM", "xterm-256color", 1);

	while (1)
	{
		loopcnt++;
		if (term == 1)
			printf("$ ");
		if ((read_status = get_line(&line, &i, stdin)) == -1)
			break;
		buff = _strtok(strdup(line), " \n");
		if (buff) /* if (buff), exec the cmd and return status code */
			exit_s = executor(
			    buff, av, get_toks(line), env, loopcnt);
	} /*
	 if (exit_s == 0)
		 exit(127); */
	return (0);
}

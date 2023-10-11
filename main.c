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
	char *line = NULL, *buff, *tmp;
	int loopcnt = 0, term = isatty(0), exit_s = 0;
	size_t i = 0; /* TODO: i variable to be renamed descriptively */
	ssize_t read_status;

	(void)ac;
	(void)exit_s;
	while (1)
	{
		loopcnt++;
		if (term == 1)
			printf("$ ");
		if ((read_status = getline(&line, &i, stdin)) == -1)
			break;
		tmp = strdup(line);
		buff = _strtok(tmp, " \n");
		if (buff) /* if (buff), exec the cmd and return status code */
			exit_s = executor(
			    buff, av, get_toks(line), env, loopcnt);
		if (shutdown(exit_s))
			exit(exit_s);
	} /*
	 if (exit_s == 0)
		 exit(127); */
	return (0);
}

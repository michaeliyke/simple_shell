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
	char *line = NULL, *buff, *argv[31];
	int status, n, loopcnt = 0, term = isatty(0);
	struct stat st;
	size_t i = 0; /* TODO: i variable to be renamed descriptively */
	ssize_t read_status;
	pid_t child;
	(void)ac;

	while (1)
	{
		loopcnt++;
		if (term == 1)
			printf("$ ");
		if ((read_status = getline(&line, &i, stdin)) == -1)
			break;
		buff = strtok(line, " \n");
		if (buff)
		{
			if (stat(buff, &st) == 0)
			{
				argv[0] = buff, n = 1;
				while (buff)
				{
					buff = strtok(NULL, " \n");
					argv[n++] = buff;
				}
				child = fork();
				if (child == 0)
				{
					if (execve(argv[0], argv, env) == -1)
					{
						perror("Execution failed");
						_exit(EXIT_FAILURE);
					}
				}
				else
				{
					wait(&status);
				}
			}
			else
			{
				dprintf(2, "%s: %d: %s: not found\n", av[0], loopcnt, buff);
			}
		}
	}
	return (0);
}

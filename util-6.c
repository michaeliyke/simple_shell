#include "main.h"

/**
 * check_unexpected - returns warning code if there's unexpected syntax
 * @line: command line input being executed
 * @ei: execution info
 *
 * Return: 0 if no unexpected syntax and warning code if otherwise
 * NOTE: if a new command starts with either || or &&, & or |,
 * then WARN_UNEXPECTED will be returned so as to discontinue the execution
 */
int check_unexpected(char *line, exec_info *ei)
{
	if (strncmp(line, "&&", 2) == 0)
	{
		free(ei->argv[0]), ei->argv[0] = strdup("&&");
		return (WARN_UNEXPECTED);
	}
	if (strncmp(line, "||", 2) == 0)
	{
		free(ei->argv[0]), ei->argv[0] = strdup("||");
		return (WARN_UNEXPECTED);
	}
	if (strncmp(line, "&", 1) == 0)
	{
		free(ei->argv[0]), ei->argv[0] = strdup("&");
		return (WARN_UNEXPECTED);
	}
	if (strncmp(line, "|", 1) == 0)
	{
		free(ei->argv[0]), ei->argv[0] = strdup("|");
		return (WARN_UNEXPECTED);
	}
	return (0);
}

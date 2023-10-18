#include "main.h"

/**
 * set_curr_bool - sets what the next boolean operator is
 * it sets bool_opt to the bool, has_bool to 1, and toggles the bools
 * @line: the command line text being checks
 * @ei: exec_info object to manipulate
 *
 * Return: void
 */
int set_curr_bool(char *line, exec_info *ei)
{
	int first_bool_char;

	ei->bool_opt = get_next_bool(line, &first_bool_char);
	ei->AND = bool_is(ei, "&&");
	ei->OR = bool_is(ei, "||");
	ei->has_bool = ei->AND || ei->OR;

	return (first_bool_char);
}

/**
 * get_next_bool - gets the next boolean operator from a string
 * @line: command line input
 * @bool_pos: this will be set to the position of the first bool character
 *
 * Return: string containing the next boolean operator or NULL
 */
char *get_next_bool(char *line, int *bool_pos)
{
	char *ch;

	if (line == NULL || *line == '\0')
		return (NULL);

	for (ch = line; *ch; ch++)
	{
		if (*ch && *(ch + 1))
		{
			if (*ch == '&' && *(ch + 1) == '&')
			{
				*bool_pos = ch - line;
				return ("&&");
			}
			else if (*ch == '|' && *(ch + 1) == '|')
			{
				*bool_pos = ch - line;
				return ("||");
			}
		}
	}
	*bool_pos = ch - line; /* Hold a ref to the pos of first bool char*/
	return (NULL);
}

/**
 * bool_is - a bool with the current value of bool_opt for a match
 * @bool_: random bool string to compare with
 * @ei: execution info
 *
 * Return: 1 if current value of bool_opt matches with bool_ and 0 otherwise
 */
int bool_is(exec_info *ei, char *bool_)
{
	return (strcmp(ei->bool_opt, bool_) == 0);
}

/**
 * get_next_boundary - gets the next command tokens for execution
 * Also adjusts the given ptr to the start of next command and
 * sets up conditional state along the way
 * @cmd: a string containing remainder of conditional commands to execute
 * @ei: execution info object to enable state caching and mgt
 *
 * Return: the string containing the current command to be executed
 * NOTE: this function basically gets the command to be executed right away,
 * changes ei values to reflect the conditional statement currently in effect,
 * and adjusts ptr to the start of next command
 */
char *get_next_boundary(exec_info *ei, char **cmd)
{
	char *p = *cmd, *out_str = *cmd; /* out_str is the returned boundary */
	int pos;

	if (*cmd == NULL || **cmd == '\0')
		return (NULL);
	/* setup ei to refelect current operation */
	pos = set_curr_bool(*cmd, ei); /* pos is at the start of || or && */
	/* Move p to start of next string: accounts for && or & */
	p += strlen(ei->bool_opt) == 2 ? pos + 2 : pos + 1;
	while (isspace(*cmd[pos - 1]))
		pos--;	     /* backwards trims trailing spaces */
	out_str[pos] = '\0'; /* Terminate the returned string */
	*cmd = p;	     /* Move ptr to start of next command if any */
	return (out_str);
}

/**
 * check_has_bool - tests if a command line has a boolean component
 * Boolean components covered are: |, ||, &, and &&
 * @line: a single command line
 *
 * Return: returns 1 for true and 0 for false
 */
int check_has_bool(char *line)
{ /* simple test of whether | or & is present in given command */
	return (str_contains('&', line) || str_contains('|', line));
}

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
	int bool_pos; /* index of first bool char if found */

	ei->curr_bool = get_next_bool(line, &bool_pos);

	if (ei->curr_bool)
		ei->last_bool = ei->curr_bool;

	return (bool_pos);
}

/**
 * get_next_bool - gets the next boolean operator from a string
 * @line: command line input
 * @bool_pos: this will be set to the position of the first bool character
 *
 * Return: string containing the next boolean operator or NULL
 * NOTE: if bool is not found, bool_pos it set to -1
 */
char *get_next_bool(char *line, int *bool_pos)
{
	char *ch = line, *ret_v = NULL;

	if (line == NULL || *line == '\0' || bool_pos == NULL)
		return (NULL);

	while (isspace(*ch)) /* skip spaces */
		ch++;

	/* Hold a ref to the position of first bool char*/
	if (*ch == '&' && *(ch + 1) == '&')
		ret_v = "&&";
	if (*ch == '|' && *(ch + 1) == '|')
		ret_v = "||";
	*bool_pos = ret_v ? (ch - line) : -1;
	return (ret_v);
}

/**
 * bool_is - tests whether the last bool encountered matches bool_
 * @bool_: random bool string to compare with
 * @ei: execution info
 *
 * Return: 1 if bool_ matches the last encountered bool and 0 otherwise
 */
int bool_is(exec_info *ei, char *bool_)
{
	if (!(ei->last_bool && bool_))
		return (0);
	return (strcmp(ei->last_bool, bool_) == 0);
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
	char *ptr, *out, *mem; /* mem is the malloc'd memory returned */
	int len;

	if (*cmd == NULL || **cmd == '\0')
		return (NULL);

	set_curr_bool(*cmd, ei); /* setup ei to correctly reflect active bool*/
	ptr = *cmd;
	/* skip leading spaces and bool chars */
	while (*ptr != '\0' && (isspace(*ptr) || *ptr == '|' || *ptr == '&'))
		ptr++;
	if (*ptr == '\0')
		return ((void *)NULL); /* empty str */

	/* Mark the boundary of string to return */
	out = ptr;
	while (*ptr != '\0' && !isspace(*ptr))
		ptr++;
	len = ptr - out;
	mem = malloc(sizeof(char) * (len + 1));
	strncpy(mem, out, len);
	mem[len] = '\0';
	*cmd = ptr;   /* Move *cmd to start of next command if any */
	return (mem); /* malloc'd */
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

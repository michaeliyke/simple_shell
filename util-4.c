#include "main.h"
/**
 * get_line - custom getline func
 * @lineptr: pointer to string where line will be stored
 * @n: number of char to be read
 * @stream: file to be ead from
 *
 * Return: umber of chars read
 */
ssize_t get_line(char **lineptr, size_t *n, FILE *stream)
{
	ssize_t bytesRead = 0;
	size_t bufferSize = *n;
	char *line = *lineptr, *newLine;
	int c;

	if (line == NULL || bufferSize == 0)
	{
		bufferSize = 255;
		line = (char *)malloc(sizeof(char) * bufferSize);
		if (line == NULL)
			return (-1);
	}
	while ((c = fgetc(stream)) != EOF)
	{
		if (bytesRead >= (ssize_t)bufferSize - 1)
		{
			bufferSize *= 2;
			newLine = (char *)realloc(line, bufferSize);
			line = newLine;
		}
		line[bytesRead++] = (char)c;
		if (c == '\n')
			break;
	}
	if (bytesRead == 0 && c == EOF)
	{
		free(line);
		return (-1);
	}
	line[bytesRead] = '\0';
	*lineptr = line;
	*n = bufferSize;

	return (bytesRead);
}

/**
 * is_digits - check if a string is a number
 * @str: the str to be ckeck
 *
 * Return: 1 for true and 0 for false
 */
int is_digits(char *str)
{
	int i = 0;

	while (str[i])
		if (!isdigit(str[i++]))
			return (0);
	return (str[0] ? 1 : 0);
}

/**
 * trunc_env - reduces the environment to a desired size
 * @new_size: the desired new size of environment
 *
 * Return: void
 */
void trunc_env(int new_size)
{
	int i;
	char **vars = malloc(sizeof(char *) * new_size);

	for (i = 0; i < new_size; i++)
		vars[i] = strdup(environ[i]);
	vars[i] = NULL;
	environ = vars;
}

/**
 * get_cmd_lines - breaks multiple command inputs separed by a semi colon into
 * into an array of command lines for better handling
 * @input: all of user input until newline; these are typically sent when the
 * the ENTER key is pressed
 * @lc: Number of different commands found in user input
 *
 * Return: and array of command lines terminated by a NULL pointer
 * NOTE: command lines are separated by a semi colon
 */
char **get_cmd_lines(char *input, int *lc)
{
	int count = 1, i = 0;
	char *buff, **cmd_lines, *ptr;

	if (input == NULL || *input == '\0')
		return (NULL);
	/* Below, we get the total number of commands */
	for (ptr = input; *ptr; ptr++)
		if (*ptr == ';')
			count++;

	cmd_lines = malloc(sizeof(char *) * (count + 2)); /* NULL inclusive */
	/* Below, we get the first command, i.e everything before the ; */
	buff = _strtok(input, ";"); /* buff is not malloc'd */

	while (buff) /* If a buffer exists, store it and check again */
	{
		cmd_lines[i++] = trim(strdup(buff)); /* store a copy of buff */
		buff = _strtok(NULL, ";");
	}
	cmd_lines[i] = NULL;
	*lc = count;
	return (cmd_lines); /* should be free'd with when done */
}

/**
 * trim - trims a string of spaced before and after
 * The trim happens in place and no extra memory ir allocated
 * @s: the input string
 *
 * Return: the original string trimmed and can be passed free'd with free
 * NOTE: must be used on a malloc'd or strdup'd string
 */
char *trim(char *s)
{
	char *front = s, *end = NULL;
	size_t len;

	if (s == NULL || *s == '\0')
		return (s == NULL ? NULL : '\0');
	len = strlen(s);
	end = s + len; /* moves end to the end of s */

	/* isspace expects non-negative values to work correctly. */
	/* if char is passed to isspace, it is converted to int */
	while (isspace((unsigned char)*front))
		front++; /* moves front to start of non-space chars */

	if (end != front) /* moves end to the last non-space char in s */
		while (isspace((unsigned char)*(--end)) && end != front)
			;

	if (s != front && end == front)
	{ /* IThe entire string is whitespace, terminate s at *s and return */
		*s = '\0';
		return (s);
	}
	else if (s + (len - 1) != end)
		*(end + 1) = '\0'; /* terminate the trimmed string at end+1 */
	/* Boundary created. Now move this boundary backwards to start at *s */
	end = s; /*Note how end has been moved to start from s */
	if (front != s)
	{
		for (end = s; *front != '\0'; end++, front++)
			*end = *front;
		*end = '\0'; /*Terminate the new string*/
	}
	return (s);
}

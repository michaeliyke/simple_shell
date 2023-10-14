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

#include "main.h"

/**
 * _strtok - retrieves string word from a string given a set separators
 * @str: source string
 * @sep: string input containing one or more separators
 *
 * Return: The first word encountered
 * first call chops off a word
 * next call must be with null
 */
char *_strtok(char *str, const char *sep)
{
	static char *next;
	char *token; /* The returned token */

	if (str) /* will be true for first execution */
		next = str;
	else if (!next || *next == '\0') /* true for last execution */
		return (NULL);
	while (*next && strchr(sep, *next))
		next++; /* skipp beyond sep chars */
	if (*next == '\0')
		return (NULL); /* return if it's the end */

	token = next; /* token startes here */
	while (*next && !strchr(sep, *next))
		next++; /* skip until the end or a sep char */
	if (*next)
	{ /* it's not the end, it's a sep char, so terminate token here */
		*next = '\0';
		next++; /* start the next token */
	}
	return (token);
}

/**
 * str_contains - check if a character is contined in a string
 * @c: the character to check
 * @str: the string to check against
 *
 * Return: 1 if true and 0 if false
 */
int str_contains(char c, char *str)
{
	return (strchr(str, c) != NULL);
}

/**
 * exit_or_cont - The program will exit here using exit code
 * @exit_code: exit status code
 *
 * Return: the exit code of last command
 */
int exit_or_cont(int exit_code)
{
	switch (exit_code)
	{
	case EXIT_NOT_FOUND:
		exit(EXIT_NOT_FOUND);
		break;
	case EXIT_ILLEGAL_NUM:
		exit(EXIT_ILLEGAL_NUM);
		break;
	case EXIT_IMMEDIATE:
		/**
		 * This comes from the exit inbuilt
		 * The default is 0 meaning no command has been ran yet
		 * (last_exit_code == INT_MAX ? 0 : last_exit_code)
		 */
		exit((last_exit_code == INT_MAX ? 0 : last_exit_code));
		break;
	}
	last_exit_code = exit_code; /* save the exit code for future use */
	return (last_exit_code);
}

/**
 * word_count - count the number of words in a string
 * @str: string of characters
 *
 * Return: count of words in a str
 *
 * NOTE: Words are delimitred by spaces and newlines only
 */
int word_count(char *str)
{
	int in_word = 0, count = 0;

	if (!str)
		return (0);
	while (*str)
	{
		if (isspace(*str)) /* if space, set in_word, and skip over */
			in_word = 0;
		else /* if not space */
		{    /** if we are in word, skip until we find a space
		      * if we find a space, update in_word and count
		      */
			if (!in_word)
				in_word = 1, count++;
		}
		str++;
	}
	return (count);
}

/**
 * free_get_toks - frees the memory allocated by get_toks function
 * @toks: pointer to string tokens
 *
 * Return: void
 */
void free_get_toks(char **toks)
{
	int i = 0;

	while (toks[i])
	{
		free(toks[i]);
		i++;
	}
	free(toks);
}

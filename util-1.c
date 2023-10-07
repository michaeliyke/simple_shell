#include "main.h"

/**
 * _setenv - sets an environment variable
 * @name: name of variable to set
 * @value: the value of the variable being set
 * @overwrite: should the variable be overwritten if exists already?
 *
 * Return: 0 if variable was set succeessfully but -1 if error occured
 */
int _setenv(const char *name, const char *value, int overwrite)
{
	/* new_env for case where environ is created fresh */
	char **ptr, **new_env, **dist; /* name=   michaliyke */
	/* + 1 below is to include the = sign in namepart */
	int namepart_len = strlen(name) + 2;
	char *namepart = malloc(sizeof(char) * namepart_len);
	char *new_entry = malloc(sizeof(char) * (namepart_len + strlen(value)));

	sprintf(namepart, "%s=", name); /* builds "name=" */
	sprintf(new_entry, "%s%s", namepart, value);
	/* Deal with case of overwrite*/
	for (ptr = environ; *ptr != NULL; ptr++)
	{
		if (strncmp(*ptr, namepart, namepart_len - 1) == 0)
		{			    /* The name exits */
			if (overwrite == 1) /* overwrite*/
				*ptr = new_entry;
			return (0);
		}
	}
	/* Deal with case where name does not exist */
	new_env = malloc(sizeof(char *) * (ptr - environ + 2));
	for (dist = new_env, ptr = environ; *ptr != NULL; ptr++, dist++)
		*dist = *ptr;
	*dist = new_entry;
	dist++;
	*dist = NULL;
	environ = new_env; /* Overwrite the old global environ */
	return (0);
}

/**
 * _unsetenv - sets an environment variable
 * @name: name of variable to set
 *
 * Return: 0 if variable was unset succeessfully but -1 if error occured
 */
int _unsetenv(const char *name)
{
	char **ptr, **new_env, **dist, *namepart;
	int found = 0;
	int name_len;

	if (name == NULL || *name == '\0')
		return (-1);
	name_len = strlen(name) + 1;
	namepart = malloc(sizeof(char) * name_len);
	sprintf(namepart, "%s=", name);

	for (ptr = environ; *ptr != NULL; ptr++)
		/* set a clip when it's found */
		if (strncmp(*ptr, namepart, name_len) == 0)
			found = 1;

	if (found == 0)
		return (0);

	new_env = malloc(sizeof(char *) * (ptr - environ + 1));
	for (ptr = environ, dist = new_env; *ptr != NULL; ptr++)
	{
		if (strncmp(*ptr, namepart, name_len) == 0)
			continue;
		*dist = *ptr;
		dist++;
	}
	*dist = NULL;
	environ = new_env;
	return (0);
}

/**
 * get_abs_path - get full path of a program
 * @name: name of program in one word
 *
 * Return: path of the program
 */
char *get_abs_path(char *name)
{
	char **dirs, *abs_path = NULL, **ptr;

	if (path_exists(name))
	{ /* Handle full and relative paths here */
		if (*name == '.')
			return realpath(name, NULL);
		return name;
	}

	/* Go through the path and search for the given  name */
	dirs = get_sys_paths(); /* All path dirs */
	if (dirs == NULL || *dirs == NULL)
		return (NULL);
	for (ptr = dirs; *ptr != NULL; ptr++)
	{ /* Allocate memory for the absolute path */
		abs_path = malloc(
		    sizeof(char) * (strlen(*ptr) + strlen(name) + 1));
		sprintf(abs_path, "%s/%s", *ptr, name);
		if (path_exists(abs_path))
			break;
		free(abs_path), abs_path = NULL;
	}
	return (abs_path); /* Here abs_path is either NULL or a full path */
}

/**
 * path_exists - checks if a given absolute path exists in system
 * @abspath: absolute or relative path
 *
 * Return: 1 for true and 0 for false
 */
int path_exists(char *abspath)
{
	struct stat st;

	if (stat(abspath, &st) == 0)
		return (1);
	return (0);
}

/**
 * _getenv - gets an env variable
 * @var_name: variable name
 *
 * Return: the value of the environment variable
 */
char *_getenv(char *var_name)
{
	size_t i;
	char **env = environ;
	char *value = NULL, *token = NULL;

	if (env == NULL || var_name == NULL)
		return (NULL);
	/* Loop through environ in search of var_name*/
	for (i = 0; env[i] != NULL; i++)
	{ /* Individual env - env[i] */
		token = _strtok(env[i], "=");
		/* token is the name part of env[i] for comaparison below */
		if (strcmp(token, var_name) == 0)
		{ /* Assign all of env[i] to data below then adjust the ptr  */
			value = env[i];
			for (i = 0; i <= strlen(token); i++)
				value++;
			/* Adjusts ptr just beyond '=' spot */
			break;
		}
	}
	return (value);
}

/**
 * _strtok - retrieves string word from a string given a set separators
 * @str: source string
 * @sep: string input containing one or more separators
 *
 * Return: The first word encountered
 * first call chops off a word
 * next call must be with null
 */
char *_strtok(char *str, char *sep)
{
	unsigned int i;
	static char *next;

	if (!str)
		str = next;
	if (str == NULL || !*str)
		return (NULL);

	/* adjust pointer past prefix sep chars*/

	/* adjust str pointer by checking is *str is a sep character */
	while (*str && str_contains(*str, sep)) /*     Michael is     just okay     */
		str++;
	if (*str == '\0')
		return (NULL); /* return if str contained only sep chars */

	for (i = 0; str[i] != '\0'; i++)
	{
		if (str_contains(str[i], sep))
		{
			/**
			 * If str[ctr] is a word sep
			 * replace it with null-terminator, this makes
			 * str become one single null-terminated word
			 * Then, below, we use next retore the next set of
			 * string chars for the benefit of the next invocation
			 */
			str[i] = '\0';
			break;
		}
	}
	next = str + i + 1; /* for the next call, next word starts here */
	return (str);
	/* null-terminated str becomaes our desired token */
}

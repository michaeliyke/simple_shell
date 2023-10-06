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

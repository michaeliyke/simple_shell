#include "main.h"

/**
 * _setenv - sets an environment variable (malloc'd)
 * @var_name: name of variable to set
 * @value: the value of the variable being set
 * @overwrite: should the variable be overwritten if exists already?
 *
 * Return: 0 if variable was set succeessfully but -1 if error occured
 * NOTE: The manual memory allocations here are not meant to be freed
 * unless a special outcome is desired
 */
int _setenv(const char *var_name, const char *value, int overwrite)
{
	char **ptr, **new_env, **dist;
	int name_len = strlen(var_name) + 2; /* + 1 for the = sign */
	char *name_eq = malloc(sizeof(char) * name_len);
	char *var = malloc(sizeof(char) * (name_len + strlen(value)));

	sprintf(name_eq, "%s=", var_name); /* makes the "name=" part */
	sprintf(var, "%s%s", name_eq, value);

	/* Deal with case of overwrite*/
	for (ptr = environ; *ptr != NULL; ptr++)
	{
		if (strncmp(*ptr, name_eq, name_len - 1) == 0)
		{ /* Does var_name= exists in the variable at *ptr ? */
			if (overwrite == 1)
			{
				free(name_eq); /* free(*ptr);*/
				*ptr = var;    /* if overwrite is set, effect it*/
				return (0);
			} /* If overwrite is not set just return success */
			free(name_eq), free(var);
			return (0);
		}
	}

	/* Deal with case where name does not exist */
	new_env = malloc(sizeof(char *) * (ptr - environ + 2));
	free(name_eq);
	for (dist = new_env, ptr = environ; *ptr != NULL; ptr++, dist++)
		*dist = *ptr;

	*dist = var;
	dist++;
	*dist = NULL;
	environ = new_env; /* Overwrite the old global environ */
	return (0);
}

/**
 * get_abs_path - get full path of a program (malloc'd)
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
			return (realpath(name, NULL)); /* malloc'd */
		if (*name == '/')
			return (name);
	}

	/* Go through the path and search for the given  name */
	dirs = get_sys_paths(); /* All path dirs */
	if (dirs == NULL)
		return (NULL);
	if (*dirs == NULL)
		return (NULL);
	for (ptr = dirs; *ptr != NULL; ptr++)
	{ /* Allocate memory for the absolute path */
		abs_path = malloc(
		    sizeof(char) * (strlen(*ptr) + strlen(name) + 2));
		sprintf(abs_path, "%s/%s", *ptr, name);
		if (path_exists(abs_path))
			break; /* break to return the malloc'd memory block */
		free(abs_path), abs_path = NULL;
	}

	free_str_arr(dirs); /* free memory allocated by get_sys_paths() */
	/* Here abs_path is either NULL or a full path */
	return (abs_path); /* malloc'd */
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
		return (*abspath == '.' || *abspath == '/');
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
	char **env = environ, *temp; /* temp mem is allocated and free'd here*/
	char *value = NULL, *token = NULL;

	if (env == NULL || var_name == NULL)
		return (NULL);
	/* Loop through environ in search of var_name*/
	for (i = 0; env[i] != NULL; i++)
	{ /* Individual env - env[i] */
		temp = strdup(env[i]);
		token = _strtok(temp, "=");
		/* token is the name part of env[i] for comaparison below */
		if (strcmp(token, var_name) == 0)
		{
			value = env[i]; /* Memory not allocated with malloc */
			/* Get the actual value of the env variable below */
			for (i = 0; i <= strlen(token); i++)
				value++; /* Adjusts ptr just beyond '=' spot */
			free(temp);
			break;
		}
		free(temp);
	}
	return (value); /* Memory block is not allocated manually */
}

/**
 * _unsetenv - unsets an environment variable for the current process
 * @name: the name of the variable to be unset
 *
 * Return: 1 for success and other int value if failed
 */
int _unsetenv(const char *name)
{
	char **ptr, **new_env, **dist, *namepart;
	int found = 0;
	int name_len;

	if (name == NULL || *name == '\0')
		return (-1);
	name_len = strlen(name);
	namepart = malloc(sizeof(char) * (name_len + 2));
	sprintf(namepart, "%s=", name);

	/* Search for varaible name in the process' env */
	for (ptr = environ; *ptr != NULL; ptr++)
		/* set a clip when it's found */
		if (strncmp(*ptr, namepart, name_len + 1) == 0)
			found = 1;
	/* If not foun, return  */
	if (found == 0)
		return (0);
	/* variable name was found, so let's remove it by rebuilding the env */
	new_env = malloc(sizeof(char *) * (ptr - environ));
	for (ptr = environ, dist = new_env; *ptr != NULL; ptr++)
	{
		if (strncmp(*ptr, namepart, name_len + 1) == 0)
			continue; /* copy over but the said variable name */
		*dist = *ptr;
		dist++;
	}
	free(namepart);
	*dist = NULL;
	environ = new_env; /* malloc'd space not to be free'd */
	return (0);
}

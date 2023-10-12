#include "main.h"

/**
 * exists - if a token word or path exists in the system
 * @token: a resource name or path
 * @abs_ptr: pointer to recieve the resolved absolute path
 *
 * Return: 1 for true and 0 for false
 * NOTE:
 * token can be an absolute or a relative path or just a program name.
 * If its is an absolute path, exits returns early with status
 * Otherwise, it is expanded to an aboslute path and status returned ...
 *   and the abs_path pointer is updated with the absolute
 */
int exists(char *token, char **abs_ptr)
{
	/* case of relative or absolute path */
	if (path_exists(token))
	{
		if (!is_program(token))
		{
			*abs_ptr = NULL;
			return (0);
		}
		/* If path is relative, get the absolute path */
		if (*token == '.')
			*abs_ptr = realpath(token, NULL);
		else if (*token == '/') /* path is aboslute */
			*abs_ptr = token;
		else
			return (0);
		return (1);
	}
	/* Case of program name */
	*abs_ptr = get_abs_path(token);
	return (*abs_ptr != NULL && is_program(*abs_ptr));
}

/**
 * is_program - check if a file resource is a program
 * @path: full relative or absolute path of resource
 *
 * Return: 1 if it's a prgram and 0 if not
 */
int is_program(char *path)
{
	struct stat st;

	if (stat(path, &st) == 0)
	{
		/**
		 * S_ISREG - checks the file mode if it is a regular file
		 * and not pip, network device, folder, etc
		 * st_mode & S_IXUSR checks if file is executable
		 */
		if (S_ISREG(st.st_mode) && (st.st_mode & S_IXUSR))
		{
			return (1);
		}
	}
	return (0);
}

/**
 * get_sys_paths - get an array of system paths
 *
 * Return: a null terminated array of system paths
 * Returns a string of strings or a NULL
 */
char **get_sys_paths()
{
	char **array, *dir, *cpy;
	int num_dirs, i;
	char *str = _getenv("PATH");

	if (str == NULL)
		return (NULL);
	cpy = strdup(str); /* dupicate original string */
	num_dirs = count_path_entr(str);
	/* allocate memory to store x number of pointer to pointer to char */
	array = malloc(sizeof(char *) * (num_dirs + 1));
	dir = _strtok(cpy, ":");
	for (i = 0; i < num_dirs && dir != NULL; i++)
	{
		array[i] = strdup(dir);
		dir = _strtok(NULL, ":");
	}
	array[i] = NULL;
	free(cpy);
	return (array);
}

/**
 * count_path_entr - count the number of dirs in a string separated by a :
 * @str: input string
 *
 * Return: total count
 */
size_t count_path_entr(char *str)
{
	char *p = str;
	size_t i, num_paths = 1;

	if (str == NULL) /* check NULL input */
		return (0);

	while (*p == ':' || *p == ' ') /* remove preceeding chars */
		p++;
	if (*p == '\0')
		return (0);
	for (i = 0; p[i] != '\0'; i++) /* count the num of : chars involved */
	{
		if (p[i] == ':' && p[i + 1] != '\0')
			num_paths++;
	}
	return (num_paths);
}

/**
 * find - indicate true or false if a resource exists in a directory
 * @dir_path: required full path of directory
 * @name: the resource name to search for
 *
 * Return: 1 if true and 0 if false, and -1 on error
 */
int find(char *dir_path, char *name)
{
	DIR *dir = opendir(dir_path);
	struct dirent *entry;

	if (dir == NULL)
		return (-1);
	while ((entry = readdir(dir)) != NULL)
	{
		if (strcmp(name, entry->d_name) == 0)
			return (1);
	}
	closedir(dir);
	return (0);
}

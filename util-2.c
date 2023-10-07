#include "main.h"

/**
 * exists - if a file exists on a given path or not
 * @path: the input file path
 *
 * Return: 1 for true and 0 for false
 */
int exists(char *path)
{
	struct stat st;

	if (stat(path, &st) == 0)
		return (1);

	/* Below we use deep search to locate path if it exists */

	return (0);
}

/**
 * get_path - get full path of program from path
 * @name: name of program in one word
 * @envp: environment variables
 *
 * Return: path of the program
 */
char *get_path(char *envp[], char *name)
{
	char **dirs, *path_str, *dir, *full_path;

	path_str = _getenv(envp, "PATH");
	if (path_str == NULL)
		return (NULL);
	dirs = get_dirs(path_str);
	if (dirs == NULL || *dirs == NULL)
		return (NULL);
	while (*dirs != NULL)
	{
		dir = *dirs;
		/**
		 * TODO: Replace find with exists
		 * To achive that use static var called visited = 0,
		 * set to 1 at the very end. Next call when basic checks fails,
		 * or succeed, ensure the detailed locate is not initiated
		 * afresh. It ends there once visited is 1
		 * Or you can use the file name instead which is better.
		 * Set that at the end and fail against that on the attempt
		 * */
		if (find(dir, name))
		{
			full_path = malloc(
			    sizeof(char) * strlen(dir) + strlen(name) + 1);
			if (full_path == NULL)
				return (NULL);
			sprintf(full_path, "%s/%s", dir, name);
			break;
		}
		dirs++;
	}
	return (is_program(full_path) ? full_path : NULL);
}

/**
 * get_env_var - gets an env variable
 * @envp: pointer to the environment
 * @var: variable name
 *
 * Return: the value of the environment variable
 */
char *_getenv(char *envp[], char *var)
{
	size_t tok_len, i;
	char *data = NULL, *token = NULL;

	if (envp == NULL || var == NULL)
		return (NULL);
	for (i = 0; envp[i] != NULL; i++)
	{
		token = _strtok(envp[i], "=");
		if (strcmp(token, var) == 0)
		{
			data = envp[i];
			tok_len = strlen(token);
			for (i = 0; i <= tok_len; i++)
				data++;
			break;
		}
	}
	return (data);
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

/**
 * is_program - check if a file resource is a program
 * @full_path: full prth of resource
 *
 * Return: 1 if it's a prgram and 0 if not
 */
int is_program(char *full_path)
{
	struct stat st;

	if (stat(full_path, &st) == 0)
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
 * get_dirs - get an array of system paths
 * @str: system path in one string
 *
 * Return: a null terminated array of systme path
 * Returns a string of strings or a NULL
 */
char **get_dirs(char *str)
{
	char **array, *dir, *cpy;
	int num_dirs, i, j;

	if (str == NULL)
		return (NULL);
	cpy = strdup(str); /* dupicate original string */
	num_dirs = count_path_entr(str);
	/* allocate memory to store x number of pointer to pointer to char */
	array = malloc(sizeof(*array) * num_dirs);
	if (array == NULL)
		return (NULL);

	dir = _strtok(cpy, ":");
	if (dir == NULL)
		return (NULL);
	array[0] = malloc(sizeof(char) * strlen(dir));
	if (array[0] == NULL)
		return (NULL);
	strcpy(array[0], dir);

	for (i = 1; i < num_dirs && dir != NULL; i++)
	{
		dir = _strtok(NULL, ":");
		if (dir == NULL)
			return (NULL);
		array[i] = malloc(sizeof(char) * strlen(dir));
		if (array[i] == NULL)
			return (NULL);
		strcpy(array[i], dir);
		j = i;
	}
	array[j + 1] = NULL;
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

/**
 * str_contains - check if a character is contined in a string
 * @c: the character to check
 * @str: the string to check against
 *
 * Return: 1 if true and 0 if false
 */
int str_contains(char c, char *str)
{
	size_t i;

	if (!str)
		return (0);
	for (i = 0; str[i]; i++)
	{
		if (str[i] == c)
			return (1);
	}
	return (0);
}

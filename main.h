#ifndef MAIN_H
#define MAIN_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <dirent.h>
#include <limits.h>
#include <ctype.h>
extern char **environ;
extern int last_exit_code;

/**
 * struct exec_info - Struct for passing command info to executor and handlers
 * @cmd_name: command name
 * @argv: command argv
 * @shell_argv: the argv originally passed to simple_shell
 * @argc: command argc
 * @env: the user environment
 * @loopcnt: variable to track the loop count from main
 */
typedef struct exec_info
{
	char *cmd_name;	   /* Name of command */
	char **argv;	   /* Command arguments */
	char **shell_argv; /* Original simple_shell arguments */
	int argc;	   /* Command arguments count */
	char **env;	   /* User environment */
	int loopcnt;	   /* Loop count control flow from main */
} exec_info;

typedef int (*builtInHandler)(exec_info info);

/**
 * struct builtin - Struct for registering a new builtin command handler
 * @name: the command name to be registered
 * @args: the command arguments, this is likely to be removed
 * @handler: the handler function to process the command
 */
typedef struct builtin
{
	char *name;		/* Command name */
	char **args;		/* Command arguments */
	builtInHandler handler; /* Command handler */
} builtin;

#define NUM_BUILTINS 5
#define EXIT_NOT_FOUND 127
#define EXIT_IMMEDIATE 7
#define EXIT_ILLEGAL_NUM 2

ssize_t get_line(char **lineptr, size_t *n, FILE *stream);
int _setenv(const char *name, const char *value, int overwrite);
int _unsetenv(const char *name);
int exists(char *token, char **abs_ptr);
int path_exists(char *abspath);
char *get_abs_path(char *name);
char *_getenv(char *var);
char *_strtok(char *str, const char *sep);
int is_program(char *full_path);
char **get_sys_paths();
size_t count_path_entr(char *str);
int find(char *dir_path, char *name);
int str_contains(char c, char *str);
#define PAPERSIZE "PAPERSIZE"
builtInHandler builtin_handler(char *command_name);
int env_fn(exec_info info);
int exit_fn(exec_info info);
int cd_fn(exec_info ei);
char **get_toks(char *s);
char *extern_handler(char *command_name);
void free_str_arr(char **arr, int limit);
int exit_or_cont(int status_code);
int is_digits(char *str);
int word_count(char *str);
int executor(exec_info info);
int exec_child(exec_info ei);
int setenv_fn(exec_info ei);
int unsetenv_fn(exec_info ei);

#endif

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

/**
 * struct alias - linked list of aliases
 * @name: name of alias
 * @value: value of alias
 * @next: pointer to next alias
 */
typedef struct alias
{
	char *name;
	char *value;
	struct alias *next;
} alias_t;

/**
 * struct exec_info - Struct for passing command info to executor and handlers
 * @cmd_name: command name
 * @argv: command argv
 * @shell_argv: the argv originally passed to simple_shell
 * @argc: command argc
 * @env: the user environment
 * @loopcnt: variable to track the loop count from main
 * @last_exit_code: exit cod of last command
 * @queued: number of commandline operations queued for processing.
 * They are each separaed with a semi-colon
 * @AND: Logical AND i.e &&
 * @OR: Logical OR i.e ||
 * @has_bool: whether a boolean operation is in effect
 * @bool_opt: the boolean operator in currently effect
 */
typedef struct exec_info
{
	char *cmd_name;	    /* Name of command */
	char **argv;	    /* Command arguments */
	char **shell_argv;  /* Original simple_shell arguments */
	int argc;	    /* Command arguments count */
	char **env;	    /* User environment */
	int loopcnt;	    /* Loop count control flow from main */
	int last_exit_code; /* exit code of last command */
	int queued;	    /* number of commands in queue */
	int AND;	    /* Logical AND i.e && */
	int OR;		    /* Logical OR i.e || */
	int has_bool;	    /* Indicate if a bool is currently in effect */
	char *bool_opt;	    /* The actual bool opt in effect */
	alias_t *al_list;   /* list of aliases */
} exec_info;

typedef int (*builtInHandler)(exec_info *info);

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

#define NUM_BUILTINS 6
#define EXIT_NOT_FOUND 127
#define EXIT_IMMEDIATE 7
#define EXIT_ILLEGAL_NUM 2
#define WARN_UNEXPECTED 14

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
int env_fn(exec_info *info);
int exit_fn(exec_info *info);
int cd_fn(exec_info *ei);
int alias_fn(exec_info *ei);
char **get_toks(char *s);
char *extern_handler(char *command_name);
void free_str_arr(char **toks);
int exit_or_cont(int status_code, exec_info *ei);
int is_digits(char *str);
int word_count(char *str);
int executor(exec_info *info);
int exec_child(exec_info *ei);
int setenv_fn(exec_info *ei);
int unsetenv_fn(exec_info *ei);
void trunc_env(int new_size);
char **get_cmd_lines(char *input, int *lc);
char *trim(char *str);
void run(char **av, char **env, exec_info *ei, char *ptr, int it);
int check_has_bool(char *line);
int check_unexpected(char *line, exec_info *ei);
int set_curr_bool(char *line, exec_info *ei);
int bool_is(exec_info *ei, char *bool_);
int run_bool(char **av, char **env, exec_info *ei, char *ptr, int lc);
char *get_next_boundary(exec_info *ei, char **cmd);
char *get_next_bool(char *line, int *bool_pos);

alias_t *add_node(alias_t **head, const char *n, const char *v);
int print_list(const alias_t *h);
void free_alias(alias_t *head);
void new_node(exec_info *ei, char *arg, char *val);
#endif

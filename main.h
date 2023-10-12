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

typedef struct
{
	char *cmd_name;
	char **argv;
	char **shell_argv;
	int argc;
	char **env;
	int loopcnt;
} exec_info;

typedef int (*builtInHandler)(exec_info info);

typedef struct
{
	char *name;
	char **args;
	builtInHandler handler;
} builtin;

#define NUM_BUILTINS 3
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
int cd_fn(char **args);
char **get_toks(char *s);
char *extern_handler(char *command_name);
void free_str_arr(char **arr, int limit);
int exit_or_cont(int status_code);
int is_digits(char *str);
int word_count(char *str);
int executor(exec_info info);

#endif

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
extern char **environ;

typedef int (*builtInHandler)(char **args);
typedef struct
{
	char *name;
	char **arguments;
	builtInHandler handler;
} builtin;

#define NUM_BUILTINS 3
#define EXIT_NOT_FOUND 127
#define EXIT_IMMEDIATE 7

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
int env_fn(char **args);
int exit_fn(char **args);
int cd_fn(char **args);
char **get_toks(char *s);
char *extern_handler(char *command_name);
int executor(char *cmd_name, char **av, char **argv, char **env, int loopcnt);
void free_str_arr(char **arr, int limit);
int exit_or_cont(int status_code);

#endif

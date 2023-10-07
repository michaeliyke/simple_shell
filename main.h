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
extern char **environ;

int _setenv(const char *name, const char *value, int overwrite);
int _unsetenv(const char *name);
int exists(char *token, char **abs_ptr);
int path_exists(char *abspath);
char *get_abs_path(char *name);
char *_getenv(char *var);
char *_strtok(char *str, char *sep);
int is_program(char *full_path);
char **get_sys_paths();
size_t count_path_entr(char *str);
int find(char *dir_path, char *name);
int str_contains(char c, char *str);
#define PAPERSIZE "PAPERSIZE"
#endif

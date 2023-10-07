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
int exists(char *path);
char *get_path(char *envp[], char *name);
char *_getenv(char *envp[], char *var);
char *_strtok(char *str, char *sep);
int is_program(char *full_path);
char **get_dirs(char *str);
size_t count_path_entr(char *str);
int find(char *dir_path, char *name);
int str_contains(char c, char *str);

#endif

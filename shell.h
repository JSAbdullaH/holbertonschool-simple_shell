#ifndef _SHELL_H
#define _SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define PROMPT "$ "
#define BUFSIZE 1024

extern char **environ;

/* Prototypes */
void display_prompt(void);
int read_line(char *buf, int size);
void strip_newline(char *str);
void execute_command(char *cmd);

#endif


#ifndef _SHELL_H
#define _SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define PROMPT "$ "
#define TOK_DELIM " \t\r\n"
#define TOK_BUFSIZE 64

extern char **environ;

/* Prototypes */
void display_prompt(void);
char *read_line(void);
char **strip_newline(char *line);
void execute_command(char **argv);
char *get_path(char *command);

#endif
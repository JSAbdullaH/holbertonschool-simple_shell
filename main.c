#include "shell.h"

/**
 * display_prompt - Prints the shell prompt to stdout
 * Only displays when stdin is connected to a terminal
 *
 * Return: void
 */
void display_prompt(void)
{
if (isatty(STDIN_FILENO))
write(STDOUT_FILENO, PROMPT, 2);
}

/**
 * read_line - Reads one line of input from stdin using getline
 *
 * Allocates a buffer via getline. The caller is responsible
 * for freeing the returned pointer. Prints a new line on
 * EOF when stdin is a terminal
 *
 * Return: Pointer to the allocated line, or NULL
 * on EOF / error
 */
char *read_line(void)
{
char *line;
size_t len;
ssize_t nread;

line = NULL;
len = 0;
nread = getline(&line, &len, stdin);
if (nread == -1)
{
free(line);
if (isatty(STDIN_FILENO))
write(STDOUT_FILENO, "\n", 1);
return (NULL);
}

return (line);
}

/**
 * strip_newline - Removes the trailing newline character
 * from a string
 * @str: The string to modify in place
 *
 * Searches for the first newline and replaces it with a null byte
 *
 * Return: void
 */
void strip_newline(char *str)
{
char *newline;

newline = strchr(str, '\n');
if (newline)
*newline = '\0';
}

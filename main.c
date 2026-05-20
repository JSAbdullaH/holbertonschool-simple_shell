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

/**
 * execute_command - Forks and executes the given command
 * @cmd: the full path to the executable to run
 *
 * Prints an error message to stderr if the executable
 * cannot be found or executed. The parent waits for the
 * child to complete.
 *
 * Return: void
 */
void execute_command(char *cmd)
{
char *argv[2];
pid_t pid;
int status = 0;

pid = fork();
if (pid == -1)
{
perror("fork");
exit(EXIT_FAILURE);
}

if (pid == 0)
{
argv[0] = cmd;
argv[1] = NULL;

if (execve(cmd, argv, environ) == -1)
{
fprintf(stderr, "./simple_shell: No such file or directory: %s\n", cmd);
exit(EXIT_FAILURE);
}

}
else
{
waitpid(pid, &status, 0);
}

}

/**
 * main - Entry point for simple shell
 *
 * Repeatedly display a prompt, reads a command, and
 * executes it until EOF is received or a read error occurs
 *
 * Return: Always 0
 */
int main(void)
{
char *line;

while (1)
{
display_prompt();

line = read_line();
if (line == NULL)
break;

strip_newline(line);

if (line[0] != '\0')
execute_command(line);

free(line);
}

return (0);
}

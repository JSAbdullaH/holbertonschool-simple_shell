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
char **strip_newline(char *line)
{

  int bufsize, position;
  char **tokens;
  char *token;

  bufsize = TOK_BUFSIZE;
  position = 0;
  tokens = malloc(sizeof(char *) * bufsize);
  if (tokens == NULL) {
    perror("malloc");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, TOK_DELIM);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += TOK_BUFSIZE;
      tokens = realloc(tokens, sizeof(char *) * bufsize);
      if (tokens == NULL) {
        perror("realloc");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, TOK_DELIM);
  }

  tokens[position] = NULL;
  return (tokens);
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
void execute_command(char **argv)
{
pid_t pid;
int status;

if (argv[0] == NULL)
  return;

pid = fork();
if (pid == -1)
{
perror("fork");
exit(EXIT_FAILURE);
}

if (pid == 0)
{

  if (execve(argv[0], argv, environ) == -1)

  {
    fprintf(stderr, "./simple_shell: 1: %s: not found\n", argv[0]);
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
char **argv;

while (1)
{
display_prompt();

line = read_line();
if (line == NULL)
break;

argv = strip_newline(line);
execute_command(argv);

free(argv);

free(line);
}

return (0);
}

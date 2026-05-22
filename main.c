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
 * get_path - Locates a command in the PATH environment variable
 * @command: The command to find
 *
 * Return: A dynamically allocated string containing the full path,
 * or NULL if the command cannot be found.
 */
char *get_path(char *command)
{
    char *path, *path_copy, *path_token, *file_path;
    int command_len, directory_len;
    struct stat buffer;

    if (command == NULL)
        return (NULL);

    /* If the command already contains a slash (e.g., /bin/ls or ./a.out) */
    if (strchr(command, '/') != NULL)
    {
        if (stat(command, &buffer) == 0)
            return (strdup(command));
        return (NULL);
    }

    path = getenv("PATH"); /* Replace with custom _getenv if required */
    if (path == NULL)
        return (NULL);

    path_copy = strdup(path);
    if (path_copy == NULL)
        return (NULL);

    command_len = strlen(command);
    path_token = strtok(path_copy, ":");

    while (path_token != NULL)
    {
        directory_len = strlen(path_token);
        file_path = malloc(command_len + directory_len + 2); /* +2 for '/' and '\0' */
        
        if (file_path == NULL)
        {
            free(path_copy);
            return (NULL);
        }

        /* Build the path: directory/command */
        strcpy(file_path, path_token);
        strcat(file_path, "/");
        strcat(file_path, command);

        /* Check if the built path exists */
        if (stat(file_path, &buffer) == 0)
        {
            free(path_copy);
            return (file_path);
        }

        free(file_path); /* Free the failed attempt */
        path_token = strtok(NULL, ":");
    }

    free(path_copy);
    return (NULL);
}

/**
 * execute_command - Forks and executes the given command
 * @argv: Array of arguments parsed from the command line
 *
 * Return: void
 */
void execute_command(char **argv)
{
    pid_t pid;
    int status;
    char *actual_cmd;

    if (argv[0] == NULL)
        return;

    /* 1. Resolve the path BEFORE forking */
    actual_cmd = get_path(argv[0]);
    if (actual_cmd == NULL)
    {
        fprintf(stderr, "./simple_shell: 1: %s: not found\n", argv[0]);
        return; /* Exit the function entirely; fork is NEVER called */
    }

    /* 2. Fork only because we know the command exists */
    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        free(actual_cmd);
        exit(EXIT_FAILURE);
    }

    if (pid == 0)
    {
        /* Child process executes the valid path */
        if (execve(actual_cmd, argv, environ) == -1)
        {
            perror(argv[0]);
            free(actual_cmd);
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        /* Parent process waits and cleans up memory */
        waitpid(pid, &status, 0);
        free(actual_cmd); 
    }
}

/**
 * main - Entry point for simple shell
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

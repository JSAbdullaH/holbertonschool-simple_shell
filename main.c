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
 * _getenv - Gets the value of an environment variable
 * @name: The name of the environment variable to find
 *
 * Return: Pointer to the value of the variable, or NULL if not found
 */
char *_getenv(const char *name)
{
    int i, j;
    int len;

    if (name == NULL || environ == NULL)
        return (NULL);

    len = strlen(name);

    for (i = 0; environ[i] != NULL; i++)
    {
        /* Check if the current environment string matches 'name' */
        for (j = 0; j < len; j++)
        {
            if (environ[i][j] != name[j])
                break;
        }

        /* If we matched the whole name and the next char is '=' */
        if (j == len && environ[i][j] == '=')
        {
            return (&environ[i][j + 1]); /* Return the value after '=' */
        }
    }

    return (NULL);
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

    if (strchr(command, '/') != NULL)
    {
        if (stat(command, &buffer) == 0)
        {
            path_copy = malloc(strlen(command) + 1);
            if (path_copy)
                strcpy(path_copy, command);
            return (path_copy);
        }
        return (NULL);
    }

    path = _getenv("PATH");
    if (path == NULL)
        return (NULL);

    path_copy = malloc(strlen(path) + 1); 
    if (path_copy == NULL)
        return (NULL);
    strcpy(path_copy, path);

    command_len = strlen(command);
    path_token = strtok(path_copy, ":");

    while (path_token != NULL)
    {
        directory_len = strlen(path_token);
        file_path = malloc(command_len + directory_len + 2);
        
        if (file_path == NULL)
        {
            free(path_copy);
            return (NULL);
        }

        strcpy(file_path, path_token);
        strcat(file_path, "/");
        strcat(file_path, command);

        if (stat(file_path, &buffer) == 0)
        {
            free(path_copy);
            return (file_path);
        }

        free(file_path);
        path_token = strtok(NULL, ":");
    }

    free(path_copy);
    return (NULL);
}

/**
 * execute_command - Forks and executes the given command
 * @argv: Array of arguments parsed from the command line
 * @shell_name: The name of the shell executable (from main's argv[0])
 *
 * Return: The exit status of the executed command, or 127 if not found
 */
int execute_command(char **argv, char *shell_name)
{
    pid_t pid;
    int status;
    char *actual_cmd;

    if (argv[0] == NULL)
        return (0);

    actual_cmd = get_path(argv[0]);
    if (actual_cmd == NULL)
    {
        /* Use the actual shell name dynamically instead of hardcoding */
        fprintf(stderr, "%s: 1: %s: not found\n", shell_name, argv[0]);
        return (127); /* Standard exit code for "command not found" */
    }

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        free(actual_cmd);
        exit(EXIT_FAILURE);
    }

    if (pid == 0)
    {
        if (execve(actual_cmd, argv, environ) == -1)
        {
            perror(shell_name);
            free(actual_cmd);
            exit(127);
        }
    }
    else
    {
        waitpid(pid, &status, 0);
        free(actual_cmd);
        
        /* Extract the actual exit code of the child process */
        if (WIFEXITED(status))
            return (WEXITSTATUS(status));
    }
    
    return (0);
}
/**
 * print_env - Prints the current environment variables
 *
 * Return: void
 */
void print_env(void)
{
    int i = 0;
    int len;

    if (environ == NULL)
        return;

    while (environ[i] != NULL)
    {
        len = strlen(environ[i]);
        write(STDOUT_FILENO, environ[i], len);
        write(STDOUT_FILENO, "\n", 1);
        i++;
    }
}

/**
 * main - Entry point for simple shell
 * @argc: Argument count
 * @argv: Argument vector
 *
 * Return: The exit status of the last executed command
 */
int main(int argc __attribute__((unused)), char **argv)
{
    char *line;
    char **cmd_argv;
    int status = 0; /* Keeps track of the exit status */

    while (1)
    {
        display_prompt();

        line = read_line();
        if (line == NULL) /* Handles EOF (Ctrl+D) */
            break;

        cmd_argv = strip_newline(line);
        
        if (cmd_argv[0] != NULL)
        {
            /* 1. Check for the "exit" built-in */
            if (strcmp(cmd_argv[0], "exit") == 0)
            {
                free(cmd_argv);
                free(line);
                exit(status);
            }
            /* 2. Check for the "env" built-in */
            else if (strcmp(cmd_argv[0], "env") == 0)
            {
                print_env();
                status = 0; /* Success exit status for env */
            }
            /* 3. Execute external commands */
            else
            {
                status = execute_command(cmd_argv, argv[0]); 
            }
        }

        /* Clean up at the end of a normal loop */
        free(cmd_argv);
        free(line);
    }

    return (status);
}

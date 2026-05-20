#include "shell.h"

/**
 * display_promt - Prints the shell promt to stdout
 * Only displays when stdin is connected to a terminal
 *
 * Return: void
 */
void display_promt(void)
{
if (isatty(STDIN_FILENO))
write(STDOUT_FILENO, PROMPT, 2);
}

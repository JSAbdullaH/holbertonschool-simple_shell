# Simple Shell (hsh)

## Description

`hsh` is a simple UNIX command-line interpreter written in C, built as the final project of the first trimester at **Holberton School**. It replicates the core behavior of `/bin/sh`, including prompt display, command parsing, `PATH` resolution, process forking with `execve`, and a small set of built-in commands.

The shell operates in two modes:
- **Interactive mode** — displays a `$ ` prompt and reads commands from the user.
- **Non-interactive mode** — reads commands from a pipe or a file (no prompt is shown).

---

## Features

- Displays a prompt and waits for user input
- Executes commands found in the `PATH` environment variable
- Supports absolute and relative paths (e.g. `/bin/ls`, `./program`)
- Handles command-line arguments
- Built-in commands: `exit`, `env`
- Handles `EOF` (`Ctrl+D`) gracefully
- Proper error messages following the format: `shell_name: line: command: not found`
- Returns the correct exit status of executed commands
- No memory leaks (all allocations are freed)

---

## Files

| File | Description |
|------|-------------|
| `main.c` | Entry point and core shell logic (prompt, read, parse, execute loop) |
| `shell.h` | Header file containing prototypes, macros, and includes |
| `AUTHORS.txt` | List of project contributors |
| `man_1_simple_shell` | Manual page for the shell |
| `README.md` | This file |

---

## Compilation

Compile all source files using `gcc` with the Holberton flags:

```bash
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh
```

This produces an executable named `hsh`.

---

## Usage

### Interactive mode

```bash
$ ./hsh
$ ls
AUTHORS.txt  README.md  hsh  main.c  shell.h
$ pwd
/home/user/simple_shell
$ exit
$
```

### Non-interactive mode

```bash
$ echo "ls -l" | ./hsh
total 48
-rw-r--r-- 1 user user   115 Nov 22 16:52 AUTHORS.txt
-rw-r--r-- 1 user user  6996 Nov 22 16:52 main.c
...
```

---

## Built-in Commands

| Command | Description |
|---------|-------------|
| `exit`  | Exits the shell with the status of the last executed command |
| `env`   | Prints all current environment variables |

---

## Examples

**Running a command:**
```
$ /bin/ls
AUTHORS.txt  main.c  README.md  shell.h
```

**Command with arguments:**
```
$ ls -la /tmp
```

**Command not found:**
```
$ wrongcommand
./hsh: 1: wrongcommand: not found
```

**Using built-ins:**
```
$ env
PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin
HOME=/home/user
...
$ exit
```

---

## Man Page

To view the manual page:

```bash
man ./man_1_simple_shell
```

---

## Authors

- **Hassan Alhuzail** — [github.com/Hassan-Alhuzali](https://github.com/Hassan-Alhuzali)
- **Abdullah Alzara** — [github.com/JSAbdullaH](https://github.com/JSAbdullaH)

---

## License

This project was developed as part of the Holberton School curriculum for educational purposes.

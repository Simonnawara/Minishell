# Minishell

&#x20;&#x20;

## 📝 Project Overview

**Minishell** is a minimalistic shell implementation built as part of the 42 School curriculum. It mimics the behavior of Bash, providing a simple command-line interface that supports various shell functionalities, including process execution, redirections, pipes, and environment variable handling.

## 🎯 Features

- ✅ Command Execution (built-in and external)
- ✅ Environment Variables (`$PATH`, `$HOME`, etc.)
- ✅ Redirections (`>`, `>>`, `<`)
- ✅ Pipes (`|`)
- ✅ Heredoc (`<<`)
- ✅ Signal Handling (`Ctrl+C`, `Ctrl+D`, `Ctrl+\`)
- ✅ Exit Status Management
- ✅ Error Handling

## 🚀 Getting Started
### Installation

```bash
# Clone the repository
git clone git@github.com:Simonnawara/Minishell.git
cd minishell

# Compile the project
make
```

### Usage

```bash
./minishell
```

You can now start using `minishell` just like a regular shell!

## 🛠 Built-in Commands

Minishell implements several built-in commands:

- `echo` - Print text to the terminal
- `cd` - Change the current directory
- `pwd` - Print working directory
- `export` - Set environment variables
- `unset` - Unset environment variables
- `env` - Print the environment variables
- `exit` - Exit the shell

## 🏗️ Project Structure

```
minishell/
│── src/
│   ├── main.c
│   ├── parser.c
│   ├── executor.c
│   ├── builtins.c
│   ├── signals.c
│── includes/
│   ├── minishell.h
│── libft/
│── Makefile
│── README.md
```

## 🔧 Features in Detail

### 🔹 Command Execution

Minishell supports execution of both built-in and external commands, managing child processes using `fork()` and `execve()`.

### 🔹 Redirections & Pipes

Supports input and output redirections (`<`, `>`, `>>`), and command piping (`|`).

### 🔹 Heredoc (`<<`)

Reads input until a specified delimiter is found.

### 🔹 Signal Handling

Handles `SIGINT` (`Ctrl+C`), `SIGQUIT` (`Ctrl+\`), and `EOF` (`Ctrl+D`).


## 🏆 Acknowledgments
This project was completed as part of the **42 School** curriculum.
</br>
Special thanks [Toiki](https://github.com/Zoroki110) for this group project.



✨ Happy Shell Scripting! 🚀


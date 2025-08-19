# Minishell

![143232233-4e385114-441f-4e3e-b3ba-477bc75454e1](https://github.com/user-attachments/assets/fdd77391-a9d4-4c0d-8aa3-d37dd4debca4)


Welcome to **Minishell**, a simple yet powerful shell implementation created as part of the 42 curriculum. This project aims to replicate a miniature version of Bash, offering hands-on experience with processes, file descriptors, and user input handling. Developed collaboratively, it’s both a technical and teamwork challenge that delivers immense learning and satisfaction.

## About the Project

Minishell is a compact shell that mimics Bash functionality on a smaller scale. It’s designed to deepen your understanding of how shells process user input, execute commands, and manage system resources. By building this project, you’ll uncover the inner workings of Bash, mastering concepts like process management, file descriptors, and environment variables. Despite its challenges, Minishell is a rewarding and fun project that fosters both technical skills and effective communication with your coding partner.

## Mind Map

<img width="1183" height="945" alt="image" src="https://github.com/user-attachments/assets/42f31c7b-58a2-45e6-a3c4-6672e3a2a4f4" />

## Code Breakdown

Here’s an overview of the key components of Minishell’s implementation:

- **Parser**: Splits user input into tokens, separating commands by pipes (`|`) and spaces, with special handling for quotes. Each token is classified as a command, argument, built-in, file, or redirection.
- **Expander**: Processes tokens to expand environment variables (e.g., `$USER`) and remove quotes. Variables are replaced with their values from the environment (`envp`), or an empty string if undefined.
- **Executor**: Assembles tokens into command arrays for `execve`. It manages multiple processes for piped commands, forks child processes, and checks `PATH` for executable paths. Built-ins run in the parent process if there’s a single command.
- **Built-ins**:
  - `echo` (supports `-n` option): Prints arguments.
  - `cd` (relative or absolute paths only): Changes directories.
  - `pwd` (no options): Displays the current directory.
  - `export` (no options): Exports environment variables.
  - `unset` (no options): Removes environment variables.
  - `env` (no options or arguments): Lists environment variables.
  - `exit` (no options): Exits Minishell.

## Supported Features

Minishell supports the following features, inspired by Bash:

- Prompt display for user input.
- Command history navigation (up and down arrows).
- Execution of system commands (e.g., `ls`, `cat`, `grep`) and local executables (e.g., `./minishell`).
- Built-in commands (see above).
- Pipes (`|`) to redirect output from one command to the input of the next.
- Redirections:
  - `>`: Redirects output.
  - `>>`: Redirects output in append mode.
  - `<`: Redirects input.
  - `<< DELIMITER`: Reads input until the specified delimiter, redirecting it to a command (no history update).
- Environment variable expansion (e.g., `$USER`, `$?` for the last exit status).
- Keyboard signals:
  - `Ctrl+C`: Displays a new prompt.
  - `Ctrl+D`: Exits Minishell.
  - `Ctrl+\`: Ignored.

## How to Use

1. **Prerequisites**: Ensure the `readline` library is installed on your system.
2. **Compile and Run**:
   ```bash
   make
   ./minishell
   ```
3. **Limitations**: Built-ins have restricted options (see descriptions above).

## Getting Started

Clone the repository, install the `readline` library, and run `make` to build the project. Launch Minishell with `./minishell` and start exploring its features!

## Acknowledgments

This project was crafted by Hugo Leonor and  [Georgii Tretiakov](https://github.com/gtretiak), cadets at 42 Porto It’s a testament to collaboration, perseverance, and the joy of learning.

Enjoy your shell adventure with Minishell! 🐚

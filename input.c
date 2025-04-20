#include "minishell.h"

char *get_input(t_mini *ms, char *prompt)
{
    char *input;
    (void)ms;
    input = readline(prompt);
    if (!input)
    {
        write(STDOUT_FILENO, "exit\n", 5);
        exit(0);
    }
    if (input && *input)
        add_history(input);
    return input;
}

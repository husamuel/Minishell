#include "./../minishell.h"

int exec_heredoc(t_token *token)
{
    int fd[2];
    char *line;
    int original_stdin;
    t_token *next;
    int status;

    next = token->next;
    if (!next || !next->cmd)
        return (127);
    if (pipe(fd) < 0)
    {
        ft_putstr_fd("minishell: erro ao criar pipe\n", 2);
        return (1);
    }
    while (1)
    {
        line = readline("> ");
        if (!line || ft_strcmp(line, next->cmd) == 0)
            break;
        ft_putstr_fd(line, fd[1]);
        ft_putstr_fd("\n", fd[1]);
        free(line);
    }
    free(line);
    close(fd[1]);
    original_stdin = dup(STDIN_FILENO);
    dup2(fd[0], STDIN_FILENO);
    close(fd[0]);
    if (next->next && next->next->type != CMD_NONE)
        status = execute_command(next->next);
    else
        status = 0;
    dup2(original_stdin, STDIN_FILENO);
    close(original_stdin);
    return (status);
}
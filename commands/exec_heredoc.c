#include "./../minishell.h"

int exec_heredoc(t_token *token, t_mini *ms)
{
    int fd[2];
    char *line;
    int original_stdin;
    t_token *next;
    t_token *cmd_token;
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
        write(fd[1], line, ft_strlen(line));
        write(fd[1], "\n", 1);
        free(line);
    }
    free(line);
    close(fd[1]);

    cmd_token = next->next;
    while (cmd_token && cmd_token->type != CMD_EXEC && cmd_token->type != CMD_BUILDIN)
        cmd_token = cmd_token->next;

    original_stdin = dup(STDIN_FILENO);
    
    dup2(fd[0], STDIN_FILENO);
    close(fd[0]);

    if (cmd_token)
        status = execute_command(cmd_token, ms);
    else if (next->next && next->next->type != CMD_NONE)
        status = execute_command(next->next, ms);
    else
        status = 0;

    dup2(original_stdin, STDIN_FILENO);
    close(original_stdin);

    return (status);
}
#include "./../minishell.h"

int exec_redirect(t_token *token)
{
    int fd;
    int status;
    int original_stdout;
    t_token *next;
    t_token *cmd_token;

    next = token->next;
    if (!next || !next->cmd)
        return (127);
    if (token->type != CMD_REDIRECT_OUT)
        return (127);

    if (ft_strcmp(token->cmd, ">") == 0)
        fd = open(next->cmd, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    else if (ft_strcmp(token->cmd, ">>") == 0)
        fd = open(next->cmd, O_WRONLY | O_CREAT | O_APPEND, 0644);
    else
        return (127);

    if (fd < 0)
    {
        ft_putstr_fd("minishell: erro ao abrir arquivo\n", 2);
        return (1);
    }

    cmd_token = token->prev;
    while (cmd_token && cmd_token->type != CMD_EXEC && cmd_token->type != CMD_BUILDIN)
        cmd_token = cmd_token->prev;

    original_stdout = dup(STDOUT_FILENO);
    
    dup2(fd, STDOUT_FILENO);
    close(fd);

    if (cmd_token)
        status = execute_command(cmd_token);
    else if (next->next && next->next->type != CMD_NONE)
        status = execute_command(next->next);
    else
        status = 0;

    dup2(original_stdout, STDOUT_FILENO);
    close(original_stdout);

    return (status);
}

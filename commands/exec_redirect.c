#include "./../minishell.h"

void build_args_from_tokens(t_token *cmd)
{
    t_token *current = cmd;
    int count = 0;

    while (current && current->type != CMD_REDIRECT_OUT && current->type != CMD_REDIRECT_IN
         && current->type != CMD_HEREDOC && current->type != CMD_PIPE)
    {
        count++;
        current = current->next;
    }

    cmd->args = malloc(sizeof(char *) * (count + 1));
    if (!cmd->args)
        return;

    current = cmd;
    int i = 0;
    while (current && i < count)
    {
        cmd->args[i++] = current->cmd;
        current = current->next;
    }
    cmd->args[i] = NULL;
}

int exec_redirect(t_token *token, t_mini *ms)
{
    int fd;
    int status;
    int original_stdout;
    t_token *next = token->next;
    t_token *cmd_token = token->prev;

    if (!next || !next->cmd || token->type != CMD_REDIRECT_OUT)
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

    while (cmd_token && cmd_token->type != CMD_EXEC && cmd_token->type != CMD_BUILDIN)
        cmd_token = cmd_token->prev;

    t_token *after_file = next->next;
    t_token *before_redir = token->prev;

    if (before_redir)
        before_redir->next = after_file;
    if (after_file)
        after_file->prev = before_redir;

    original_stdout = dup(STDOUT_FILENO);
    dup2(fd, STDOUT_FILENO);
    close(fd);

    if (cmd_token)
        build_args_from_tokens(cmd_token);

    if (cmd_token)
        status = execute_command(cmd_token, ms);
    else if (after_file && after_file->type != CMD_NONE)
        status = execute_command(after_file, ms);
    else
        status = 0;

    dup2(original_stdout, STDOUT_FILENO);
    close(original_stdout);

    if (before_redir)
        before_redir->next = token;
    token->prev = before_redir;

    token->next = next;
    next->prev = token;
    next->next = after_file;
    if (after_file)
        after_file->prev = next;

    return (status);
}

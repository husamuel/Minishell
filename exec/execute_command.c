#include "./../minishell.h"

int is_valid_command(t_token *token)
{
    if (!token || !token->cmd || token->type == CMD_NONE)
        return (0);
    if (token->is_invalid || !token->args || !token->args[0])
        return (0);
    return (1);
}

int execute_simple_command(t_token *token, t_mini *ms)
{
    if (!is_valid_command(token))
        return (1);

    if (token->type == CMD_BUILDIN)
        return (exec_builtin(token, ms));
    else if (token->type == CMD_EXPR)
    {
        process_expr_command(token, ms);
        return (0);
    }
    return (0);
}



int execute_external_command(t_token *token, t_mini *ms)
{
    pid_t pid;
    int status;
    char *cmd_path;

    if (!is_valid_command(token))
    {
        ft_putstr_fd("minishell: command not found\n", 2);
        return (127);
    }

    cmd_path = find_command_path(token->cmd, ms->export);
    if (!cmd_path)
    {
        ft_putstr_fd("minishell: command not found: ", 2);
        ft_putstr_fd(token->cmd, 2);
        ft_putstr_fd("\n", 2);
        return (127);
    }

    pid = fork();
    if (pid == 0)
    {
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);

        if (setup_redirections(token) == -1)
            exit(1);

        if (execve(cmd_path, token->args, get_env_array(ms->export)) == -1)
        {
            perror(token->cmd);
            free(cmd_path);
            exit(127);
        }
    }
    else if (pid > 0)
    {
        waitpid(pid, &status, 0);
        free(cmd_path);
        return (WEXITSTATUS(status));
    }
    else
    {
        perror("fork");
        free(cmd_path);
        return (1);
    }
    return (0);
}
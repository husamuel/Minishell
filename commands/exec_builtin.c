#include "./../minishell.h"

static int is_builtin(const char *cmd)
{
    if (!cmd)
        return (0);
    static const char *builtins[] = {
        "cd", "echo", "pwd", "export",
        "unset", "env", "exit", "/bin/pwd",
        "/bin/env", "/bin/echo", NULL
    };
    const char **check = builtins;
    while (*check)
    {
        if (strcmp(cmd, *check) == 0)
            return (1);
        check++;
    }
    return (0);
}

int exec_builtin(t_token *token, t_mini *mini)
{
    int status;

    status = 127;
    if (!is_builtin(token->cmd))
        return status;        
    if (strcmp(token->cmd, "cd") == 0)
        exec_cd(token, mini);
    else if ((strcmp(token->cmd, "echo") == 0) || (strcmp(token->cmd, "/bin/echo") == 0))
        exec_echo(token, mini);
    else if ((strcmp(token->cmd, "pwd") == 0) || (strcmp(token->cmd, "/bin/pwd") == 0))
        exec_pwd(token);
    else if (strcmp(token->cmd, "export") == 0)
        exec_export(token, mini);
    else if (strcmp(token->cmd, "unset") == 0)
        exec_unset(token, mini);
    else if ((strcmp(token->cmd, "env") == 0) || (strcmp(token->cmd, "/bin/env") == 0))
        exec_env(token, mini);
    else if (strcmp(token->cmd, "exit") == 0)
        exec_exit(token);
    return status;
}

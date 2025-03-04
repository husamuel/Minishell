#include "./../minishell.h"

static int is_builtin(const char *cmd)
{
    static const char *builtins[] = {
        "cd", "echo", "pwd", "export",
        "unset", "env", "exit", NULL
    };
    int i;

    i = 0;
    while (builtins[i])
    {
        if (strcmp(cmd, builtins[i]) == 0)
            return (1);
        i++;
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
    else if (strcmp(token->cmd, "echo") == 0)
        exec_echo(token, mini);
    else if (strcmp(token->cmd, "pwd") == 0)
        exec_pwd(token);
    else if (strcmp(token->cmd, "export") == 0)
        exec_export(token, mini);
    else if (strcmp(token->cmd, "unset") == 0)
        exec_unset(token, mini);
    else if (strcmp(token->cmd, "env") == 0)
        exec_env(token, mini);
    else if (strcmp(token->cmd, "exit") == 0)
        exec_exit(token);
    return status;
}

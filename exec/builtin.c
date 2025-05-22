#include "./../minishell.h"

int execute_external_builtin(t_token *token, t_mini *ms)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid == 0)
    {
        if (setup_redirections(token) == -1)
            exit(1);
        exec_builtin(token, ms);  // não retorna
        exit(0); // builtins não têm retorno direto
    }
    else if (pid > 0)
    {
        waitpid(pid, &status, 0);
        return (WEXITSTATUS(status));
    }
    else
    {
        perror("fork");
        return (1);
    }
}

int is_builtin(const char *cmd)
{
    int i;
    static const char *builtins[] = {
        "cd", "echo", "pwd", "export", "unset", "env", "exit",
        "/bin/pwd", "/bin/env", "/bin/echo", NULL
    };

    if (!cmd)
        return (0);
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
    char *s[3];

    status = 127;
    s[0] = "/bin/echo";
    s[1] = "/bin/pwd";
    s[2] = "/bin/env";
    if (!is_builtin(token->cmd))
        return (status);
    if (strcmp(token->cmd, "cd") == 0)
        exec_cd(token, mini);
    else if ((strcmp(token->cmd, "echo") == 0) || !strcmp(token->cmd, s[0]))
        exec_echo(token, mini);
    else if ((strcmp(token->cmd, "pwd") == 0) || !strcmp(token->cmd, s[1]))
        exec_pwd(token);
    else if (strcmp(token->cmd, "export") == 0)
        exec_export(token, mini);
    else if (strcmp(token->cmd, "unset") == 0)
        exec_unset(token, mini);
    else if ((strcmp(token->cmd, "env") == 0) || !strcmp(token->cmd, s[2]))
        exec_env(token, mini);
    else if (strcmp(token->cmd, "exit") == 0)
        exec_exit(token, mini);
    return (status);
}

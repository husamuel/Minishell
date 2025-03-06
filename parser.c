#include "minishell.h"

int is_builtin_command(const char *cmd)
{
    static const char *builtins[] = {
        "cd", "echo", "pwd", "export",
        "unset", "env", "exit", NULL
    };
    const char **check;

    check = builtins;
    while (*check)
    {
        if (strcmp(cmd, *check) == 0)
            return (1);
        check++;
    }
    return (0);
}

int is_exec_command(const char *cmd)
{
    static const char *executables[] = {
        "ls", "cat", "mkdir", "rm", "cp", "mv",
        "grep", "wc", "ps", "head", "tail",
        "sort", "clear", "touch", NULL
    };
    const char **check;

    check = executables;
    while (*check)
    {
        if (strcmp(cmd, *check) == 0)
            return (1);
        check++;
    }
    return (0);
}

void set_command_type(t_token *current)
{
    if (is_builtin_command(current->cmd))
        current->type = CMD_BUILDIN;
    else
        current->type = CMD_EXEC;
}


void parser(t_mini *ms)
{
    t_token *current;
    t_token *prev;
    t_token *last_cmd;
    int command_seen;

    if (!ms || !ms->token)
        return ;

    current = ms->token;
    prev = NULL;
    last_cmd = NULL;
    command_seen = 0;

    while (current)
    {
        process_token(current, prev, &last_cmd, &command_seen, ms);
        prev = current;
        current = current->next;
    }
}

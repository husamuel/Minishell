#include "./../minishell.h"

void exec(t_mini *ms)
{
    if (!ms || !ms->token)
    {
        ms->exit_status = 0;
        return;
    }

    // Set up signal handling for parent
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);

    if (ms->pipe > 0)
        ms->exit_status = execute_pipeline(ms);
    else
    {
        t_token *current = ms->token;
        
        while (current)
        {
            if (is_valid_command(current))
                ms->exit_status = execute_token(current, ms);
            current = current->next;
        }
    }

    // Restore default signal handling
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
}
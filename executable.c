#include "minishell.h"

void exec(t_mini *ms)
{
	t_token *current;
    
    current = ms->token;
    while (current)
    {
        if (current->type == CMD_PIPE)
            exec_pipe(current);
        else if (current->type == CMD_EXEC)
            return ;
        else if (current->type == CMD_BUILDIN)
            exec_builtin(current, ms);
        else if (current->type == CMD_REDIRECT)
            exec_redirect(current);
        else if (current->type == CMD_HEREDOC)
            exec_heredoc(current);
        current = current->next;
    }
}
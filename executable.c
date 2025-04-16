#include "minishell.h"

void exec(t_mini *ms)
{
    t_token *current;
    t_token *prev;

    current = ms->token;
    prev = NULL;
    if (ms->pipe != 0)
    {
        ms->exit_status = exec_pipe(ms);
    }
    else
    {
        while (current)
        {
            if (current->type == CMD_BUILDIN)
                ms->exit_status = exec_builtin(current, ms);
            else if (current->type == CMD_REDIRECT_OUT)
                ms->exit_status = exec_redirect(current);
            else if (current->type == CMD_REDIRECT_IN)
                ms->exit_status = exec_redirect(current);
            else if (current->type == CMD_HEREDOC)
                ms->exit_status = exec_heredoc(current);
            else if (current->type == CMD_EXPR)
                process_expr_command(current, ms);
            else if (current->type == CMD_EXEC)
                ms->exit_status = execute_command(current);
            else if (current->type == CMD_NONE)
                ms->none = 1;
            else if (current->type == CMD_EXIT_STATUS && (!prev || prev->type != CMD_EXPR))
            {
                printf("command not found: %d\n", ms->exit_status);
                ms->exit_status = 127;
            }
            else if ((!prev || prev->type != CMD_EXPR) && current->type != CMD_PLUS)
            {
                printf("command not found: %s\n", current->cmd);
                ms->exit_status = 127;
                return ;
            }
            prev = current;
            current = current->next;
        }
    }
}
 
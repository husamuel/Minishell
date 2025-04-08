#include "minishell.h"

void exec(t_mini *ms)
{
    t_token *current;

    current = ms->token;
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
			else if (current->type == CMD_SUBSHELL)
                ms->exit_status = execute_command(current);
            else if (current->type == CMD_EXIT_STATUS)
                printf("command not found: %d\n", ms->exit_status);
            
            current = current->next;
        }
    }
}

#include "minishell.h"

void exec(t_mini *ms)
{
    t_token *current;
    int status;

    current = ms->token;
    while (current)
    {
        if (current->type == CMD_PIPE)
            status = exec_pipe(ms);
        else if (current->type == CMD_BUILDIN)
            status = exec_builtin(current, ms);
        else if (current->type == CMD_REDIRECT)
            status = exec_redirect(current);
        else if (current->type == CMD_HEREDOC)
            status = exec_heredoc(current);
        else if (current->type == CMD_EXPR)
            process_expr_command(current, ms);
        else if (current->type == CMD_EXEC)
            status = execute_command(current, ms);
		else if (current->type == CMD_EXIT_STATUS)
			printf("command not found: %d\n", ms->exit_status);
        
        current = current->next;
    }
    handle_exit_status(ms, status);
}

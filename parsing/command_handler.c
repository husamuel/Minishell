#include "./../minishell.h"

void display_command_not_found(t_token *token, t_mini *ms)
{
    ft_putstr_fd("minishell: ", 2);
    ft_putstr_fd(token->cmd, 2);
    ft_putstr_fd(": command not found\n", 2);
    ms->exit_status = 127;
	ms->expr = 1;
}

void handle_command_token(t_token *current, t_token **last_cmd,
                        int *command_seen, t_mini *ms)
{
    current->args_file = malloc(sizeof(char *) * 2);
    if (!current->args_file)
        return;
    current->args_file[0] = ft_strdup(current->cmd);
    current->args_file[1] = NULL;
    current->args = malloc(sizeof(char *) * 2);
    if (!current->args)
        return;
    current->args[0] = ft_strdup(current->cmd);
    current->args[1] = NULL;
    
    if (is_builtin_command(current->cmd) || is_exec_command(current->cmd))
    {
        set_command_type(current);
        *command_seen = 1;
        *last_cmd = current;
    }
    else
    {
        ms->none = 1;
        current->type = CMD_NONE;
        display_command_not_found(current, ms);
    }
}

void handle_argument_token(t_token *current, t_token *prev, t_token *last_cmd, t_mini *ms)
{
    char *processed_arg;

    (void)ms;
    
    if (!last_cmd)
        return;
        
    processed_arg = ft_strdup(current->cmd);
    if (processed_arg)
    {
        if (prev && (is_redirect_in(prev->cmd) || is_redirect_out(prev->cmd)))
        {
            add_to_args_file(last_cmd, processed_arg);
            current->type = CMD_ARG_FILE;
        }
        else
        {
            add_to_args(last_cmd, processed_arg);
            current->type = CMD_ARG;
            
        }
        free(processed_arg);
    }
}

void	process_token(t_token *current, t_token *prev, t_token **last_cmd,
	int *command_seen, t_mini *ms)
{
	process_quotes(current);
	if (prev && prev->cmd && prev->cmd[0] == '|')
	{
		*command_seen = 0;
		*last_cmd = NULL;
	}
	if (is_math_operator(current))
		return ;
	if (ft_strcmp(current->cmd, "$?") == 0 && !is_in_expr_context(prev))
	{
		process_exit_status(current, ms);
		if (!(*command_seen))
		{
			setup_command_after_exit_status(current, command_seen, last_cmd);
		}
		else if (*last_cmd)
			handle_argument_token(current, prev, *last_cmd, ms);
		return ;
	}
	else if (ft_strcmp(current->cmd, "$?") == 0 && is_in_expr_context(prev))
	{
		ms->exit_status_count++;
		if (*last_cmd)
			handle_argument_token(current, prev, *last_cmd, ms);
		return ;
	}
	process_token_part2(current, prev, last_cmd, command_seen, ms);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 18:09:09 by gtretiak          #+#    #+#             */
/*   Updated: 2025/04/27 18:54:38 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

void	display_command_not_found(t_token *token, t_mini *ms)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(token->cmd, 2);
	ft_putstr_fd(": command not found\n", 2);
	ms->exit_status = 127;
	ms->expr = 1;
}

void	handle_command_token(t_token *current, t_token **last_cmd,
		int *command_seen, t_mini *ms) //TODO
{
	current->args_file = malloc(sizeof(char *) * 2);
	if (!current->args_file)
		return ;
	current->args_file[0] = ft_strdup(current->cmd);
	current->args_file[1] = NULL;
	current->args = malloc(sizeof(char *) * 2);
	if (!current->args)
		return ;
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

void	handle_arg_token(t_token *curr, t_token *prev, t_token *last, t_mini *ms)//TODO
{
	char	*processed_arg;

	(void)ms;
	if (!last)
		return ;
	processed_arg = ft_strdup(curr->cmd);
	if (processed_arg)
	{
		if (prev && (is_redirect_in(prev->cmd) || is_redirect_out(prev->cmd)))
		{
			add_to_args_file(last, processed_arg);
			curr->type = CMD_ARG_FILE;
		}
		else
		{
			add_to_args(last, processed_arg);
			curr->type = CMD_ARG;
		}
		free(processed_arg);
	}
}

void	process_token(t_token *current, t_token *prev, t_token **last_cmd,
	int *command_seen, t_mini *ms) //TODO
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
			handle_arg_token(current, prev, *last_cmd, ms);
		return ;
	}
	else if (ft_strcmp(current->cmd, "$?") == 0 && is_in_expr_context(prev))
	{
		ms->exit_status_count++;
		if (*last_cmd)
			handle_arg_token(current, prev, *last_cmd, ms);
		return ;
	}
	process_token_part2(current, prev, last_cmd, command_seen, ms);
}

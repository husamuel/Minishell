/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_processing.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 13:31:33 by gtretiak          #+#    #+#             */
/*   Updated: 2025/05/02 16:03:02 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

void setup_command_after_exit_status(t_parser *state)
{
	state->cmd_seen = 1;
	state->last_cmd = state->curr;
	
	if (!state->curr->args)
	{
		state->curr->args = malloc(sizeof(char *) * 2);
		if (!state->curr->args)
			return;
		
		state->curr->args[0] = ft_strdup(state->curr->cmd);
		if (!state->curr->args[0])
		{
			free(state->curr->args);
			state->curr->args = NULL;
			return;
		}
		state->curr->args[1] = NULL;
	}
	
	if (!state->curr->args_file)
	{
		state->curr->args_file = malloc(sizeof(char *) * 2);
		if (!state->curr->args_file)
			return;
		
		state->curr->args_file[0] = ft_strdup(state->curr->cmd);
		if (!state->curr->args_file[0])
		{
			free(state->curr->args_file);
			state->curr->args_file = NULL;
			return;
		}
		state->curr->args_file[1] = NULL;
	}
}

static void ft_decide_on_exit_status(t_parser *state, t_mini *ms)
{
	if (!is_in_expr_context(state->prev))
	{
		process_exit_status(state->curr, ms);
		if (!state->cmd_seen)
			setup_command_after_exit_status(state);
		else if (state->last_cmd)
			handle_arg_token(state, ms);
	}
	else
	{
		ms->exit_status_count++;
		if (state->last_cmd)
			handle_arg_token(state, ms);
	}
}

void process_token(t_parser *state, t_mini *ms)
{
	if (!state->curr)
		return;
	process_quotes(state->curr);
	if (state->prev && state->prev->cmd && state->prev->cmd[0] == '|')
	{
		state->cmd_seen = 0;
		state->last_cmd = NULL;
	}
	
	if (is_math_operator(state->curr))
		return;
	
	if (state->curr->cmd && ft_strcmp(state->curr->cmd, "$?") == 0)
	{
		ft_decide_on_exit_status(state, ms);
		return;
	}
	
	process_token_part2(state, ms);
}

void process_token_part2(t_parser *state, t_mini *ms)
{
	if (!state->curr)
		return;
		
	ft_handle_spec(state, ms);
	
	if (!state->cmd_seen)
	{
		handle_command_token(state, ms);
	}
	else if(ms->token && ms->token->type != CMD_EXPR && ms->token->type != CMD_REDIRECT_IN && ms->token->type != CMD_REDIRECT_OUT && ms->token->type != CMD_PIPE)
	{
		ft_handle_norm(state, ms);
	}
}
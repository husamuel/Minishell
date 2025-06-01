/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_spec_handler.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 13:35:09 by gtretiak          #+#    #+#             */
/*   Updated: 2025/06/01 15:15:04 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

void	ft_handle_spec(t_parser *state, t_mini *ms)
{
	if (ft_strcmp(state->curr->cmd, "expr") == 0)
	{
		setup_expr_command(state->curr, &state->cmd_seen, &state->last_cmd);
		return ;
	}
	if (state->curr->cmd[0] == '|')
		setup_pipe_token(state->curr, ms);
	else if (is_append(state->curr->cmd))
		setup_append_token(state->curr, ms);
	else if (is_redirect_out(state->curr->cmd))
		setup_redirect_out_token(state->curr, ms);
	else if (is_heredoc(state->curr->cmd))
		setup_heredoc_in_token(state->curr, ms);
	else if (is_redirect_in(state->curr->cmd))
		setup_redirect_in_token(state->curr, ms);
	else if (state->curr->cmd[0] == '\\' || state->curr->cmd[0] == ';')
		state->curr->type = CMD_NONE;
}

void	ft_handle_norm(t_parser *state, t_mini *ms)
{
	char	*processed_arg;

	if (state->prev && (state->prev->type == CMD_REDIRECT_IN
			|| state->prev->type == CMD_REDIRECT_OUT
			|| state->prev->type == CMD_HEREDOC
			|| state->prev->type == CMD_APPEND))
	{
		processed_arg = ft_strdup(state->curr->cmd);
		if (processed_arg)
		{
			add_to_args_file(state->last_cmd, processed_arg);
			state->curr->type = CMD_ARG_FILE;
			free(processed_arg);
		}
	}
	else if (state->curr->type != CMD_HEREDOC
		&& state->curr->type != CMD_REDIRECT_IN
		&& state->curr->type != CMD_REDIRECT_OUT
		&& state->curr->type != CMD_PIPE
		&& state->curr->type != CMD_APPEND)
	{
		handle_arg_token(state, ms);
	}
}

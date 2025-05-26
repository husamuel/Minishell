/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_processing2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 18:56:02 by husamuel          #+#    #+#             */
/*   Updated: 2025/05/26 09:38:20 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

void	process_token_part2(t_parser *state, t_mini *ms)
{
	if (!state->curr)
		return ;
	ft_handle_spec(state, ms);
	if (!state->cmd_seen)
	{
		handle_command_token(state, ms);
	}
	else if (is_valid_for_processing(ms))
	{
		ft_handle_norm(state, ms);
	}
}

void	process_token(t_parser *state, t_mini *ms)
{
	if (!state->curr)
		return ;
	process_quotes(state->curr);
	if (state->prev && state->prev->cmd && state->prev->cmd[0] == '|')
	{
		state->cmd_seen = 0;
		state->last_cmd = NULL;
	}
	if (is_math_operator(state->curr))
		return ;
	if (state->curr->cmd && ft_strcmp(state->curr->cmd, "$?") == 0)
	{
		ft_decide_on_exit_status(state, ms);
		return ;
	}
	if (state->curr->cmd
		&& ft_strncmp(state->curr->cmd, "./minishell", 11) == 0)
	{
		free(state->curr->cmd);
		state->curr->cmd = ft_strdup("./minishell");
		state->curr->type = CMD_EXEC;
	}
	process_token_part2(state, ms);
}

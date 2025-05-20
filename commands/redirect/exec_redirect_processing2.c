/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirect_processing2.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 18:31:10 by husamuel          #+#    #+#             */
/*   Updated: 2025/05/20 18:33:44 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

static t_token	*ft_find_cmd(t_token *token, t_token *after)
{
	t_token	*cmd;

	cmd = token->prev;
	while (cmd && cmd->type != CMD_EXEC && cmd->type != CMD_BUILDIN)
		cmd = cmd->prev;
	if (!cmd && after && after->type != CMD_NONE)
		cmd = after;
	return (cmd);
}

int	exec_redirect(t_token *token, t_mini *ms)
{
	t_token	*next;
	t_token	*cmd;
	t_token	*after;
	int		is_input;
	int		fd_and_status[2];

	while (token && (token->type == CMD_REDIRECT_IN
			|| token->type == CMD_REDIRECT_OUT || token->type == CMD_PIPE))
	{
		next = token->next;
		is_input = (token->type == CMD_REDIRECT_IN);
		fd_and_status[0] = ft_open_redirect_file(token, next);
		if (fd_and_status[0] < 0)
			return (1);
		else if (fd_and_status[0] == -127)
			return (fd_and_status[0] * -1);
		after = ft_unlink_tokens(token, next);
		cmd = ft_find_cmd(token, after);
		fd_and_status[1] = ft_redirect_execution(cmd, ms,
				fd_and_status[0], is_input);
		ft_relink_tokens(token, next, after);
		token = next;
	}
	return (fd_and_status[1]);
}

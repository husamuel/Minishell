/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_processor3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 10:17:48 by husamuel          #+#    #+#             */
/*   Updated: 2025/06/01 10:08:52 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

int	is_redirect_in(char *cmd)
{
	return (cmd[0] == '<');
}

void	setup_redirect_out_token(t_token *current, t_mini *ms)
{
	current->type = CMD_REDIRECT_OUT;
	++ms->redirect;
}

void	setup_append_token(t_token *current, t_mini *ms)
{
	current->type = CMD_APPEND;
	++ms->redirect;
}

void	handle_exit_status_argument(t_token *current, t_token *last_cmd)
{
	char	*exit_status_str;

	exit_status_str = ft_itoa(g_exit_status);
	if (exit_status_str)
	{
		add_to_args_file(last_cmd, exit_status_str);
		add_to_args(last_cmd, exit_status_str);
		current->type = CMD_ARG;
		free(exit_status_str);
	}
}

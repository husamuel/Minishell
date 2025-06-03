/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 10:21:32 by husamuel          #+#    #+#             */
/*   Updated: 2025/06/03 15:08:21 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

static void	exec_end(t_mini *ms)
{
	t_token	*current;

	current = ms->token;
	while (current)
	{
		if (is_valid_command(current, ms))
		{
			restore_child_tty();
			g_exit_status = execute_token(current, ms);
		}
		else
			g_exit_status = 127;
		current = current->next;
	}
}

void	exec(t_mini *ms)
{
	if (!ms || !ms->token)
	{
		g_exit_status = 0;
		return ;
	}
	setup_exec_signals();
	if (ms->pipe > 0)
	{
		g_exit_status = execute_pipeline(ms);
	}
	else
		exec_end(ms);
}

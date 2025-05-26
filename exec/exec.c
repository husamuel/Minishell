/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 10:21:32 by husamuel          #+#    #+#             */
/*   Updated: 2025/05/25 11:20:37 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

void	exec(t_mini *ms)
{
	t_token		*current;

	if (!ms || !ms->token)
	{
		ms->exit_status = 0;
		return ;
	}
	setup_exec_signals();
	if (ms->pipe > 0)
		ms->exit_status = execute_pipeline(ms);
	else
	{
		current = ms->token;
		while (current)
		{
			if (is_valid_command(current))
			{
				restore_child_tty();
				ms->exit_status = execute_token(current, ms);
			}
			else
				ms->exit_status = 127;
			current = current->next;
		}
	}
	reset_signals();
}

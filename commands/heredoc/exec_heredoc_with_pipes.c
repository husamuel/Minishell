/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc_with_pipes.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 16:25:47 by husamuel          #+#    #+#             */
/*   Updated: 2025/05/20 16:50:42 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

int	exec_heredoc_with_pipes(t_mini *ms)
{
	t_pipe_ctx	ctx;
	int			status;

	if (!ms || !ms->token)
	{
		ft_putstr_fd("minishell: invalid input\n", 2);
		return (1);
	}
	ctx.cmd_index = 0;
	ctx.count = ms->pipe;
	ctx.pipe_fds = init_pipes2(ctx.count);
	if (!ctx.pipe_fds)
	{
		ft_putstr_fd("minishell: pipe initialization failed\n", 2);
		return (1);
	}
	status = execute_pipeline(ms, &ctx);
	if (status != 0)
		return (status);
	close_pipes(&ctx);
	return (update_exit_status(ms, ms->exit_status));
}

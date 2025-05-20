/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_with_redirect3.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 18:38:15 by husamuel          #+#    #+#             */
/*   Updated: 2025/05/20 18:48:08 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

static int	process_pipeline_segment(t_token **current, t_mini *ms,
	t_pipe_ctx *ctx)
{
	pid_t	pid;
	int		redirect_status;
	t_token	*next_cmd;

	if (!current || !*current)
		return (1);
	redirect_status = setup_redirects(*current, &next_cmd);
	if (redirect_status != 0)
		return (redirect_status);
	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		close_and_free_pipes1(ctx);
		return (127);
	}
	if (pid == 0)
		handle_child_process1(*current, ms, ctx);
	close_pipe_ends(ctx);
	*current = next_cmd;
	return (0);
}

static void	process_wait_status(t_mini *ms)
{
	int	status;

	while (wait(&status) > 0)
	{
		if (WIFEXITED(status))
			ms->exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			ms->exit_status = WTERMSIG(status) + 128;
	}
}

static int	process_pipeline_tokens(t_mini *ms, t_pipe_ctx *ctx)
{
	t_token	*current;
	int		status;

	current = ms->token;
	while (current)
	{
		if (current->type == CMD_PIPE)
		{
			current = current->next;
			continue ;
		}
		status = process_pipeline_segment(&current, ms, ctx);
		if (status != 0)
		{
			close_and_free_pipes1(ctx);
			return (status);
		}
		while (current && current->type != CMD_PIPE)
			current = current->next;
		ctx->cmd_index++;
	}
	return (0);
}

int	exec_pipe_with_redirects(t_mini *ms)
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
	ctx.pipe_fds = init_pipes1(ctx.count);
	if (!ctx.pipe_fds)
	{
		ft_putstr_fd("minishell: pipe initialization failed\n", 2);
		return (1);
	}
	status = process_pipeline_tokens(ms, &ctx);
	if (status != 0)
		return (status);
	close_and_free_pipes1(&ctx);
	process_wait_status(ms);
	return (ms->exit_status);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_pipeline.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 14:35:49 by gtretiak          #+#    #+#             */
/*   Updated: 2025/04/27 14:37:15 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

static t_token	*skip_pipe_tokens(t_token *current)
{
	while (current && current->type == CMD_PIPE)
		current = current->next;
	return (current);
}

static int	update_exit_status(t_mini *ms, int status)
{
	while (wait(&status) > 0)
	{
		if (WIFEXITED(status))
			ms->exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			ms->exit_status = WTERMSIG(status) + 128;
	}
	return (status);
}

static int	ft_process_cmd(t_token **current, t_mini *ms, t_pipe_ctx *ctx)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		close_and_free_pipes(ctx);
		return (127);
	}
	if (pid == 0)
		handle_child_process(*current, ms, ctx);
	return (0);
}

static int	ft_pipeline_execution(t_mini *ms, t_pipe_ctx *ctx)
{
	int		status;
	t_token	*current;

	status = 0;
	current = ms->token;
	while (current)
	{
		if (current->type == CMD_PIPE || current->type == CMD_ARG)
		{
			current = current->next;
			continue ;
		}
		if (ft_process_cmd(&current, ms, ctx) == 127)
			return (127);
		current = skip_pipe_tokens(current->next);
		ctx->cmd_index++;
	}
	return (status);
}

int	exec_pipe(t_mini *ms)
{
	int			status;
	t_pipe_ctx	ctx;

	status = 0;
	ctx.cmd_index = 0;
	ctx.count = ms->pipe;
	ctx.pipe_fds = init_pipes(ctx.count);
	if (!ctx.pipe_fds)
		return (1);
	status = ft_pipeline_execution(ms, &ctx);
	close_and_free_pipes(&ctx);
	return (update_exit_status(ms, status));
}

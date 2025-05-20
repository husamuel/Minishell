/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_pipeline_exec.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 17:24:02 by husamuel          #+#    #+#             */
/*   Updated: 2025/05/20 17:24:04 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../minishell.h"

t_token	*next_command(t_token *current)
{
	while (current && current->type == CMD_PIPE)
		current = current->next;
	return (current);
}

static void	close_pipe_fds(t_pipe_ctx *ctx)
{
	int	i;

	i = 0;
	while (i < ctx->count)
	{
		if (ctx->pipe_fds[i])
		{
			if (ctx->cmd_index > 0 && i == ctx->cmd_index - 1)
				close(ctx->pipe_fds[i][0]);
			if (ctx->cmd_index < ctx->count && i == ctx->cmd_index)
				close(ctx->pipe_fds[i][1]);
		}
		i++;
	}
}

int	process_command(t_token **current, t_mini *ms, t_pipe_ctx *ctx)
{
	pid_t	pid;
	t_token	*start_token;

	if (!current || !*current)
		return (1);
	start_token = *current;
	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		close_pipes(ctx);
		return (127);
	}
	if (pid == 0)
		execute_child(start_token, ms, ctx);
	close_pipe_fds(ctx);
	while (*current && (*current)->type != CMD_PIPE)
		*current = (*current)->next;
	*current = next_command(*current);
	return (0);
}

int	execute_pipeline(t_mini *ms, t_pipe_ctx *ctx)
{
	t_token	*current;
	int		status;

	current = ms->token;
	status = 0;
	ctx->cmd_index = 0;
	while (current)
	{
		status = process_command(&current, ms, ctx);
		if (status != 0)
		{
			close_pipes(ctx);
			return (status);
		}
		ctx->cmd_index++;
	}
	return (status);
}

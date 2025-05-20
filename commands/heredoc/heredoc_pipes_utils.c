/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_pipes_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 17:23:56 by husamuel          #+#    #+#             */
/*   Updated: 2025/05/20 17:23:58 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../minishell.h"

static void	free_pipe_fds(int **pipe_fds, int count)
{
	while (--count >= 0)
	{
		close(pipe_fds[count][0]);
		close(pipe_fds[count][1]);
		free(pipe_fds[count]);
	}
	free(pipe_fds);
}

int	**init_pipes2(int pipe_count)
{
	int	**pipe_fds;
	int	i;

	pipe_fds = malloc(sizeof(int *) * pipe_count);
	if (!pipe_fds)
		return (NULL);
	i = 0;
	while (i < pipe_count)
	{
		pipe_fds[i] = malloc(sizeof(int) * 2);
		if (!pipe_fds[i])
			return (free_pipe_fds(pipe_fds, i), NULL);
		if (pipe(pipe_fds[i]) == -1)
			return (free_pipe_fds(pipe_fds, i + 1), NULL);
		i++;
	}
	return (pipe_fds);
}

void	close_pipes(t_pipe_ctx *ctx)
{
	int	i;

	if (!ctx || !ctx->pipe_fds)
		return ;
	i = 0;
	while (i < ctx->count)
	{
		if (ctx->pipe_fds[i])
		{
			close(ctx->pipe_fds[i][0]);
			close(ctx->pipe_fds[i][1]);
			free(ctx->pipe_fds[i]);
		}
		i++;
	}
	free(ctx->pipe_fds);
	ctx->pipe_fds = NULL;
}

void	dup_pipe_ends(int **pipe_fds, int cmd_index,
					int pipe_count, int has_output_redirect)
{
	if (cmd_index > 0)
	{
		if (dup2(pipe_fds[cmd_index - 1][0], STDIN_FILENO) == -1)
		{
			perror("minishell: dup2 stdin");
			exit(1);
		}
	}
	if (cmd_index < pipe_count && !has_output_redirect)
	{
		if (dup2(pipe_fds[cmd_index][1], STDOUT_FILENO) == -1)
		{
			perror("minishell: dup2 stdout");
			exit(1);
		}
	}
}

void	close_all_pipe_fds(int **pipe_fds, int pipe_count)
{
	int	i;

	i = 0;
	while (i < pipe_count)
	{
		if (pipe_fds[i])
		{
			close(pipe_fds[i][0]);
			close(pipe_fds[i][1]);
		}
		i++;
	}
}

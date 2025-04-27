/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 14:37:24 by gtretiak          #+#    #+#             */
/*   Updated: 2025/04/27 14:44:55 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

void	close_and_free_pipes(t_pipe_ctx *ctx)
{
	int	i;

	i = 0;
	while (i < ctx->count)
	{
		close(ctx->pipe_fds[i][0]);
		close(ctx->pipe_fds[i][1]);
		free(ctx->pipe_fds[i]);
		i++;
	}
	free(ctx->pipe_fds);
}

static void	free_pipes_on_error(int **pipe_fds, int index)
{
	int	j;

	j = 0;
	while (j <= index)
	{
		free(pipe_fds[j]);
		j++;
	}
	free(pipe_fds);
}

int	**init_pipes(int pipe_count)
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
		{
			free_pipes_on_error(pipe_fds, i - 1);
			return (NULL);
		}
		if (pipe(pipe_fds[i]) == -1)
		{
			perror("minishell: pipe");
			free_pipes_on_error(pipe_fds, i);
			return (NULL);
		}
		i++;
	}
	return (pipe_fds);
}

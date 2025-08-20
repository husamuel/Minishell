/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 17:50:37 by husamuel          #+#    #+#             */
/*   Updated: 2025/05/23 18:03:55 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

int	count_commands(t_token *tokens)
{
	char	**pipes_cmd;
	int		count;

	if (!tokens || !tokens->pipes_cmd)
		return (0);
	pipes_cmd = tokens->pipes_cmd;
	count = 0;
	while (pipes_cmd[count])
		count++;
	return (count);
}

static void	cleanup_pipes(int **pipes, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		free(pipes[i]);
		i++;
	}
	free(pipes);
}

static int	**allocate_and_create_pipes(int cmd_count)
{
	int	**pipes;
	int	i;

	pipes = malloc(sizeof(int *) * (cmd_count - 1));
	if (!pipes)
		return (NULL);
	i = 0;
	while (i < cmd_count - 1)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i] || pipe(pipes[i]) == -1)
		{
			cleanup_pipes(pipes, i);
			return (NULL);
		}
		i++;
	}
	return (pipes);
}

int	**create_pipes(int cmd_count)
{
	if (cmd_count <= 1)
		return (NULL);
	return (allocate_and_create_pipes(cmd_count));
}

void	close_all_pipes(t_pipe_ctx *ctx)
{
	int	i;

	if (!ctx || !ctx->pipe_fds)
		return ;
	i = 0;
	while (i < ctx->count - 1)
	{
		if (ctx->pipe_fds[i])
		{
			if (ctx->pipe_fds[i][0] != -1)
				close(ctx->pipe_fds[i][0]);
			if (ctx->pipe_fds[i][1] != -1)
				close(ctx->pipe_fds[i][1]);
			free(ctx->pipe_fds[i]);
		}
		i++;
	}
	free(ctx->pipe_fds);
	ctx->pipe_fds = NULL;
}

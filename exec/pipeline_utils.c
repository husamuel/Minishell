/* ************************************************************************** */
/*                                                                            */
/*                            :::      ::::::::   */
/*   pipeline_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 18:00:00 by husamuel          #+#    #+#             */
/*   Updated: 2025/05/22 18:05:00 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

/**
 * Conta os comandos presentes em pipes_cmd[]
 */
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

/**
 * Cria os pipes necessários entre os comandos.
 * Para N comandos, precisamos de N-1 pipes.
 */
int	**create_pipes(int cmd_count)
{
	int	**pipes;
	int	i;

	if (cmd_count <= 1)
		return (NULL);
	pipes = malloc(sizeof(int *) * (cmd_count - 1));
	if (!pipes)
		return (NULL);
	i = 0;
	while (i < cmd_count - 1)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i] || pipe(pipes[i]) == -1)
		{
			while (--i >= 0)
			{
				close(pipes[i][0]);
				close(pipes[i][1]);
				free(pipes[i]);
			}
			free(pipes);
			return (NULL);
		}
		i++;
	}
	return (pipes);
}

void	close_all_pipes(t_pipe_ctx *ctx)
{
	int	i;

	if (!ctx || !ctx->pipe_fds)
		return;

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

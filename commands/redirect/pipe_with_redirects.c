/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_with_redirects.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 18:45:33 by husamuel          #+#    #+#             */
/*   Updated: 2025/05/20 18:45:34 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

int	**init_pipes_alloc(int count)
{
	int	**pipe_fds;
	int	i;

	pipe_fds = malloc(sizeof(int *) * count);
	if (!pipe_fds)
		return (NULL);
	i = 0;
	while (i < count)
	{
		pipe_fds[i] = malloc(sizeof(int) * 2);
		if (!pipe_fds[i])
			return (NULL);
		i++;
	}
	return (pipe_fds);
}

int	**init_pipes1(int count)
{
	int	**pipe_fds;
	int	i;

	pipe_fds = init_pipes_alloc(count);
	if (!pipe_fds)
		return (NULL);
	i = 0;
	while (i < count)
	{
		if (pipe(pipe_fds[i]) == -1)
		{
			while (--i >= 0)
			{
				close(pipe_fds[i][0]);
				close(pipe_fds[i][1]);
				free(pipe_fds[i]);
			}
			free(pipe_fds);
			return (NULL);
		}
		i++;
	}
	return (pipe_fds);
}

void	close_and_free_pipes1(t_pipe_ctx *ctx)
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

int	process_redirect_file(t_token *token, t_token *next)
{
	int	fd;

	fd = ft_open_redirect_file(token, next);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: error opening file\n", 2);
		return (1);
	}
	if (fd == -127)
		return (127);
	return (fd);
}

int	setup_redirect_single(t_token *token, t_token *next,
	t_token **after)
{
	int	fd;
	int	is_input;
	int	original_fd;

	is_input = (token->type == CMD_REDIRECT_IN);
	fd = process_redirect_file(token, next);
	if (fd <= 0)
		return (fd);
	original_fd = ft_redir_exec_setup(fd, is_input);
	*after = ft_unlink_tokens(token, next);
	if (is_input)
		dup2(original_fd, STDIN_FILENO);
	else
		dup2(original_fd, STDOUT_FILENO);
	close(original_fd);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 17:47:56 by husamuel          #+#    #+#             */
/*   Updated: 2025/05/23 17:48:59 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

static int	handle_output_redirect(t_token *current)
{
	int	fd;

	fd = open(current->next->cmd, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0 || dup2(fd, STDOUT_FILENO) == -1)
	{
		perror(current->next->cmd);
		return (-1);
	}
	close(fd);
	return (0);
}

static int	handle_append_redirect(t_token *current)
{
	int	fd;

	fd = open(current->next->cmd, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0 || dup2(fd, STDOUT_FILENO) == -1)
	{
		perror(current->next->cmd);
		return (-1);
	}
	close(fd);
	return (0);
}

static int	handle_input_redirect(t_token *current)
{
	int	fd;

	fd = open(current->next->cmd, O_RDONLY);
	if (fd < 0 || dup2(fd, STDIN_FILENO) == -1)
	{
		perror(current->next->cmd);
		return (-1);
	}
	close(fd);
	return (0);
}

static int	handle_heredoc_redirect(t_token *current)
{
	int	fd;

	fd = setup_heredoc(current->next->cmd);
	if (fd < 0 || dup2(fd, STDIN_FILENO) == -1)
	{
		perror("heredoc");
		return (-1);
	}
	close(fd);
	return (0);
}

int	process_redirection(t_token *current)
{
	if (current->type == CMD_REDIRECT_OUT && current->next)
		return (handle_output_redirect(current));
	else if (current->type == CMD_APPEND && current->next)
		return (handle_append_redirect(current));
	else if (current->type == CMD_REDIRECT_IN && current->next)
		return (handle_input_redirect(current));
	else if (current->type == CMD_HEREDOC && current->next)
		return (handle_heredoc_redirect(current));
	return (0);
}

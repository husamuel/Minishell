/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirect_processing.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 13:22:50 by gtretiak          #+#    #+#             */
/*   Updated: 2025/04/27 13:23:02 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

int	ft_open_redirect_file(t_token *token, t_token *next)
{
	int	fd;

	if (!next || !next->cmd)
		return (-127);
	if (token->type == CMD_REDIRECT_OUT)
	{
		if (ft_strcmp(token->cmd, ">") == 0)
			fd = open(next->cmd, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (ft_strcmp(token->cmd, ">>") == 0)
			fd = open(next->cmd, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			return (-127);
	}
	else if (token->type == CMD_REDIRECT_IN)
	{
		if (ft_strcmp(token->cmd, "<") == 0)
			fd = open(next->cmd, O_RDONLY);
		else
			return (-127);
	}
	else
		return (-127);
	if (fd < 0)
		ft_putstr_fd("minishell: error opening file\n", 2);
	return (fd);
}

void	ft_relink_tokens(t_token *token, t_token *next, t_token *after)
{
	t_token	*before;

	before = token->prev;
	if (before)
		before->next = token;
	token->next = next;
	next->prev = token;
	next->next = after;
	if (after)
		after->prev = next;
}

int	ft_redir_exec_setup(int fd, int is_input)
{
	int	original_fd;

	if (is_input)
	{
		original_fd = dup(STDIN_FILENO);
		dup2(fd, STDIN_FILENO);
	}
	else
	{
		original_fd = dup(STDOUT_FILENO);
		dup2(fd, STDOUT_FILENO);
	}
	close(fd);
	return (original_fd);
}

int	ft_redirect_execution(t_token *cmd, t_mini *ms,
		int fd, int is_input)
{
	int	status;
	int	original_fd;

	original_fd = ft_redir_exec_setup(fd, is_input);
	if (cmd)
	{
		build_args_from_tokens(cmd);
		status = execute_command(cmd, ms);
	}
	else
		status = 0;
	if (is_input)
		dup2(original_fd, STDIN_FILENO);
	else
		dup2(original_fd, STDOUT_FILENO);
	close(original_fd);
	return (status);
}

t_token	*ft_unlink_tokens(t_token *token, t_token *next)
{
	t_token	*after;

	after = next->next;
	if (token->prev)
		token->prev->next = after;
	if (after)
		after->prev = token->prev;
	return (after);
}

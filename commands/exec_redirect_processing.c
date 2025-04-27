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

static int	ft_open_redirect_file(t_token *token, t_token *next)
{
	int		fd;

	if (!next || !next->cmd || token->type != CMD_REDIRECT_OUT)
		return (-127);
	if (ft_strcmp(token->cmd, ">") == 0)
		fd = open(next->cmd, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (ft_strcmp(token->cmd, ">>") == 0)
		fd = open(next->cmd, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		return (-127);
	if (fd < 0)
		ft_putstr_fd("minishell: error openning file\n", 2);
	return (fd);
}

static void	ft_relink_tokens(t_token *token, t_token *next, t_token *after)
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

static int	ft_redirect_execution(t_token *cmd, t_mini *ms, int fd)
{
	int	status;
	int	original_stdout;

	original_stdout = dup(STDOUT_FILENO);
	dup2(fd, STDOUT_FILENO);
	close(fd);
	if (cmd)
	{
		build_args_from_tokens(cmd);
		status = execute_command(cmd, ms);
	}
	else
		status = 0;
	dup2(original_stdout, STDOUT_FILENO);
	close(original_stdout);
	return (status);
}

static t_token	*ft_unlink_tokens(t_token *token, t_token *next)
{
	t_token	*after;

	after = next->next;
	if (token->prev)
		token->prev->next = after;
	if (after)
		after->prev = token->prev;
	return (after);
}

int	exec_redirect(t_token *token, t_mini *ms)
{
	int		fd_and_status[2];
	t_token	*next;
	t_token	*cmd;
	t_token	*after;

	next = token->next;
	cmd = token->prev;
	fd_and_status[0] = ft_open_redirect_file(token, next);
	if (fd_and_status[0] < 0)
		return (1);
	else if (fd_and_status[0] == -127)
		return (fd_and_status[0] * -1);
	while (cmd && cmd->type != CMD_EXEC
		&& cmd->type != CMD_BUILDIN)
		cmd = cmd->prev;
	after = ft_unlink_tokens(token, next);
	if (!cmd && after && after->type != CMD_NONE)
		cmd = after;
	fd_and_status[1] = ft_redirect_execution(cmd, ms, fd_and_status[0]);
	ft_relink_tokens(token, next, after);
	return (fd_and_status[1]);
}

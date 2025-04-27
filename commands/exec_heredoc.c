/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 18:37:03 by gtretiak          #+#    #+#             */
/*   Updated: 2025/04/27 12:26:11 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

static int	ft_handle_heredoc_input(t_token *next, int fd[2])
{
	char	*line;

	if (!next || !next->cmd)
		return (127);
	if (pipe(fd) < 0)
	{
		ft_putstr_fd("minishell: error creating pipe\n", 2);
		return (1);
	}
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, next->cmd) == 0)
			break ;
		write(fd[1], line, ft_strlen(line));
		write(fd[1], "\n", 1);
		free(line);
	}
	free(line);
	close(fd[1]);
	return (0);
}

static int	ft_heredoc_execution(t_token *cmd_token, t_mini *ms, int fd)
{
	int		original_stdin;
	int		status;

	original_stdin = dup(STDIN_FILENO);
	dup2(fd, STDIN_FILENO);
	close(fd);
	if (cmd_token)
		status = execute_command(cmd_token, ms);
	else
		status = 0;
	dup2(original_stdin, STDIN_FILENO);
	close(original_stdin);
	return (status);
}

int	exec_heredoc(t_token *token, t_mini *ms)
{
	int		fd[2];
	t_token	*next;
	t_token	*cmd_token;
	int		heredoc_status;

	next = token->next;
	heredoc_status = ft_handle_heredoc_input(next, fd);
	if (heredoc_status != 0)
		return (heredoc_status);
	cmd_token = next->next;
	while (cmd_token && cmd_token->type != CMD_EXEC
		&& cmd_token->type != CMD_BUILDIN)
		cmd_token = cmd_token->next;
	if (!cmd_token && next->next && next->next->type != CMD_NONE)
		cmd_token = next->next;
	return (ft_heredoc_execution(cmd_token, ms, fd[0]));
}

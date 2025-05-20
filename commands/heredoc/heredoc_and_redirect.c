/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_and_redirect.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 17:24:13 by husamuel          #+#    #+#             */
/*   Updated: 2025/05/20 17:24:14 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../minishell.h"

static int	open_heredoc_pipe(char *delimiter, int fd[2])
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(fd[1], line, ft_strlen(line));
		write(fd[1], "\n", 1);
		free(line);
	}
	return (0);
}

int	setup_heredoc(t_token *token, t_mini *ms)
{
	t_token	*delimiter_token;
	int		fd[2];

	(void)ms;
	if (!token || !token->cmd || ft_strcmp(token->cmd, "<<") != 0)
	{
		ft_putstr_fd("minishell: invalid heredoc token\n", 2);
		return (-127);
	}
	delimiter_token = token->next;
	if (!delimiter_token || !delimiter_token->cmd)
	{
		ft_putstr_fd("minishell: no delimiter provided\n", 2);
		return (-127);
	}
	if (pipe(fd) < 0)
		return (ft_putstr_fd("minishell: pipe error\n", 2), -1);
	open_heredoc_pipe(delimiter_token->cmd, fd);
	close(fd[1]);
	if (dup2(fd[0], STDIN_FILENO) == -1)
		return (perror("minishell: dup2 heredoc"), close(fd[0]), -1);
	close(fd[0]);
	return (0);
}

int	handle_redirect(t_token *token, t_mini *ms)
{
	int	fd;

	(void)ms;
	if (!token->next || !token->next->cmd)
	{
		ft_putstr_fd("minishell: invalid redirect\n", 2);
		return (-1);
	}
	if (token->type == CMD_REDIRECT_OUT)
	{
		fd = open(token->next->cmd, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1 || dup2(fd, STDOUT_FILENO) == -1)
			return (perror("minishell: redir out"), close(fd), -1);
		close(fd);
	}
	else if (token->type == CMD_REDIRECT_IN)
	{
		fd = open(token->next->cmd, O_RDONLY);
		if (fd == -1 || dup2(fd, STDIN_FILENO) == -1)
			return (perror("minishell: redir in"), close(fd), -1);
		close(fd);
	}
	return (0);
}

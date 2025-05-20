/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 18:12:08 by husamuel          #+#    #+#             */
/*   Updated: 2025/05/20 18:12:08 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

int	ft_handle_heredoc_input(const t_token *next, int fd[2])
{
	char	*line;

	if (!next || !next->cmd)
		return (127);
	if (pipe(fd) < 0)
	{
		ft_putstr_fd("minishell: pipe error\n", 2);
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

int	ft_execute_builtin(t_token *cmd_token, t_mini *ms, int fd)
{
	int	original_stdin;
	int	status;

	original_stdin = dup(STDIN_FILENO);
	dup2(fd, STDIN_FILENO);
	close(fd);
	status = exec_builtin(cmd_token, ms);
	dup2(original_stdin, STDIN_FILENO);
	close(original_stdin);
	return (status);
}

int	ft_execute_external(t_token *cmd_token, t_mini *ms, int fd)
{
	int	original_stdin;
	int	status;

	original_stdin = dup(STDIN_FILENO);
	dup2(fd, STDIN_FILENO);
	close(fd);
	status = execute_command(cmd_token, ms);
	dup2(original_stdin, STDIN_FILENO);
	close(original_stdin);
	return (status);
}

int	ft_heredoc_default(int fd)
{
	int		original_stdin;
	int		bytes_read;
	char	buffer[4096];

	original_stdin = dup(STDIN_FILENO);
	dup2(fd, STDIN_FILENO);
	close(fd);
	bytes_read = read(STDIN_FILENO, buffer, 4095);
	while (bytes_read > 0)
	{
		buffer[bytes_read] = '\0';
		ft_putstr_fd(buffer, STDOUT_FILENO);
		bytes_read = read(STDIN_FILENO, buffer, 4095);
	}
	dup2(original_stdin, STDIN_FILENO);
	close(original_stdin);
	return (0);
}

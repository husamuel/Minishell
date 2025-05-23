/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 17:48:22 by husamuel          #+#    #+#             */
/*   Updated: 2025/05/23 18:05:56 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

int	setup_redirections(t_token *token)
{
	t_token	*current;

	current = token->next;
	while (current)
	{
		if (process_redirection(current) == -1)
			return (-1);
		current = current->next;
	}
	return (0);
}

static int	check_delimiter_match(char *line, char *delimiter)
{
	size_t	delim_len;

	if (strcmp(line, delimiter) == 0)
		return (1);
	delim_len = strlen(delimiter);
	if (strncmp(line, delimiter, delim_len) == 0
		&& line[delim_len] == '\n')
		return (1);
	return (0);
}

static int	read_heredoc_line(int write_fd, char *delimiter)
{
	char	*line;
	size_t	len;
	char	*prompt;

	prompt = "> ";
	line = NULL;
	len = 0;
	write(STDOUT_FILENO, prompt, strlen(prompt));
	if (getline(&line, &len, stdin) == -1)
	{
		free(line);
		return (-1);
	}
	if (check_delimiter_match(line, delimiter))
	{
		free(line);
		return (-1);
	}
	write(write_fd, line, strlen(line));
	free(line);
	return (0);
}

int	setup_heredoc(char *delimiter)
{
	int	fd[2];

	if (!delimiter)
		return (-1);
	if (pipe(fd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	while (1)
	{
		if (read_heredoc_line(fd[1], delimiter) == -1)
		{
			close(fd[1]);
			return (fd[0]);
		}
	}
}

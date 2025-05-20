/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 18:16:52 by husamuel          #+#    #+#             */
/*   Updated: 2025/05/20 18:21:19 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

static t_token	*ft_find_cmd_token(const t_token *token)
{
	t_token	*current;

	current = token->prev;
	while (current && current->type != CMD_EXEC && current->type != CMD_BUILDIN)
		current = current->prev;
	return (current);
}

static int	validate_heredoc_token(t_token *token)
{
	if (!token || !token->cmd || ft_strcmp(token->cmd, "<<") != 0)
	{
		ft_putstr_fd("minishell: invalid heredoc token\n", 2);
		return (127);
	}
	if (!token->next || !token->next->cmd)
	{
		ft_putstr_fd("minishell: no delimiter provided\n", 2);
		return (127);
	}
	return (0);
}

static int	resolve_cmd_token(t_token *token, int fd[2])
{
	t_token	*cmd_token;

	cmd_token = ft_find_cmd_token(token);
	if (!cmd_token || !cmd_token->cmd)
	{
		ft_putstr_fd("minishell: no command found\n", 2);
		return (ft_heredoc_default(fd[0]));
	}
	if (cmd_token->type == CMD_EXEC)
		return (ft_execute_external(cmd_token, NULL, fd[0]));
	if (cmd_token->type == CMD_BUILDIN)
		return (ft_execute_builtin(cmd_token, NULL, fd[0]));
	ft_putstr_fd("minishell: invalid command type\n", 2);
	return (ft_heredoc_default(fd[0]));
}

int	exec_heredoc(t_token *token, t_mini *ms)
{
	int			fd[2];
	int			status;

	(void)ms;
	status = validate_heredoc_token(token);
	if (status != 0)
		return (status);
	status = ft_handle_heredoc_input(token->next, fd);
	if (status != 0)
		return (status);
	return (resolve_cmd_token(token, fd));
}

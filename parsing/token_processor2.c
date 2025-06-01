/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_processor2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 15:25:21 by gtretiak          #+#    #+#             */
/*   Updated: 2025/06/01 15:12:23 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

void	setup_redirect_in_token(t_token *current, t_mini *ms)
{
	current->type = CMD_REDIRECT_IN;
	++ms->redirect;
}

void	setup_heredoc_in_token(t_token *current, t_mini *ms)
{
	current->type = CMD_HEREDOC;
	++ms->redirect;
}

void	process_exit_status(t_token *current)
{
	char	*exit_status_str;

	current->type = CMD_EXIT_STATUS;
	free(current->cmd);
	exit_status_str = ft_itoa(g_exit_status);
	current->cmd = exit_status_str;
	if (current->args)
	{
		free(current->args[0]);
		current->args[0] = ft_strdup(exit_status_str);
	}
	if (current->args_file)
	{
		free(current->args_file[0]);
		current->args_file[0] = ft_strdup(exit_status_str);
	}
}

int	is_in_expr_context(t_token *prev)
{
	if (prev && prev->cmd && ft_strcmp(prev->cmd, "expr") == 0)
		return (1);
	while (prev)
	{
		if (ft_strcmp(prev->cmd, "expr") == 0)
			return (1);
		if (prev->type == CMD_PIPE || prev->type == CMD_REDIRECT_IN
			|| prev->type == CMD_REDIRECT_OUT || prev->type == CMD_HEREDOC)
			break ;
		prev = prev->prev;
	}
	return (0);
}

int	is_math_operator(t_token *current)
{
	if (ft_strcmp(current->cmd, "+") == 0
		|| ft_strcmp(current->cmd, "-") == 0
		|| ft_strcmp(current->cmd, "*") == 0
		|| ft_strcmp(current->cmd, "/") == 0
		|| ft_strcmp(current->cmd, "%") == 0)
	{
		current->type = CMD_PLUS;
		return (1);
	}
	return (0);
}

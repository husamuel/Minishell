/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_processor2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 15:25:21 by gtretiak          #+#    #+#             */
/*   Updated: 2025/04/27 15:27:06 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

void	setup_redirect_in_token(t_token *current, t_mini *ms)
{
	if (current->cmd[1] == '<')
		current->type = CMD_HEREDOC;
	else
		current->type = CMD_REDIRECT_IN;
	++ms->redirect;
}

void	process_exit_status(t_token *current, t_mini *ms)
{
	char	*exit_status_str;

	current->type = CMD_EXIT_STATUS;
	free(current->cmd);
	exit_status_str = ft_itoa(ms->exit_status);
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

void	handle_exit_status_argument(t_token *current, t_token *last_cmd,
	t_mini *ms)
{
	char	*exit_status_str;

	exit_status_str = ft_itoa(ms->exit_status);
	if (exit_status_str)
	{
		add_to_args_file(last_cmd, exit_status_str);
		add_to_args(last_cmd, exit_status_str);
		current->type = CMD_ARG;
		free(exit_status_str);
	}
}

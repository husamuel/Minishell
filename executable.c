/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executable.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 13:28:59 by gtretiak          #+#    #+#             */
/*   Updated: 2025/04/21 13:48:18 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int	ft_handle_token(t_token *current, t_token *prev, t_mini *ms)
{
	if (current->type == CMD_BUILDIN)
		ms->exit_status = exec_builtin(current, ms);
	else if (current->type == CMD_EXPR)
		process_expr_command(current, ms);
	else if (current->type == CMD_EXEC)
		ms->exit_status = execute_command(current, ms);
	else if (current->type == CMD_EXIT_STATUS
		&& (!prev || prev->type != CMD_EXPR))
	{
		printf("command not found: %d\n", ms->exit_status);
		ms->exit_status = 127;
	}
	else if ((!prev || prev->type != CMD_EXPR) && current->type != CMD_PLUS)
	{
		ms->exit_status = 127;
		return (1);
	}
	return (0);
}

static void	process_special_token(t_token *special_token, t_mini *ms)
{
	if (special_token->type == CMD_REDIRECT_OUT)
		ms->exit_status = exec_redirect(special_token, ms);
	else if (special_token->type == CMD_REDIRECT_IN)
		ms->exit_status = exec_redirect(special_token, ms);
	else if (special_token->type == CMD_HEREDOC)
		ms->exit_status = exec_heredoc(special_token, ms);
}

static t_token	*find_special_token(t_token *current)
{
	t_token	*temp;
	t_token	*special_token;

	special_token = NULL;
	temp = current->next;
	while (temp && temp->type != CMD_PIPE
		&& temp->type != CMD_BUILDIN && temp->type != CMD_EXEC)
	{
		if (temp->type == CMD_REDIRECT_OUT || temp->type == CMD_REDIRECT_IN
			|| temp->type == CMD_HEREDOC)
		{
			special_token = temp;
			break ;
		}
		temp = temp->next;
	}
	return (special_token);
}

void	ft_exec_token_list(t_mini *ms)
{
	t_token	*current;
	t_token	*prev;
	t_token	*temp;
	t_token	*special_token;

	current = ms->token;
	prev = NULL;
	while (current)
	{
		if (current->type == CMD_BUILDIN || current->type == CMD_EXEC)
		{
			special_token = find_special_token(current);
			if (special_token)
			{
				process_special_token(special_token, ms);
				temp = current->next;
				while (current && current != temp)
					current = current->next;
				prev = current ? current->prev : NULL;
				continue ;
			}
		}
		if (ft_handle_token(current, prev, ms))
			return ;
		prev = current;
		current = current->next;
	}
}

void	exec(t_mini *ms)
{
	if (ms->pipe != 0)
		ms->exit_status = exec_pipe(ms);
	else
		ft_exec_token_list(ms);
}
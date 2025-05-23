/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_processor.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 15:27:55 by gtretiak          #+#    #+#             */
/*   Updated: 2025/05/23 10:20:56 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

void	setup_expr_command(t_token *current, int *command_seen,
	t_token **last_cmd)
{
	current->type = CMD_EXPR;
	*command_seen = 1;
	*last_cmd = current;
	current->args_file = malloc(sizeof(char *) * 2);
	if (!current->args_file)
		return ;
	current->args_file[0] = ft_strdup(current->cmd);
	current->args_file[1] = NULL;
	current->args = malloc(sizeof(char *) * 2);
	if (!current->args)
		return ;
	current->args[0] = ft_strdup(current->cmd);
	current->args[1] = NULL;
}

void	setup_pipe_token(t_token *current, t_mini *ms)
{
	current->type = CMD_PIPE;
	++ms->pipe;
}

int	is_append(char *cmd)
{
	return (ft_strlen(cmd) >= 2 && cmd[0] == '>' && cmd[1] == '>');
}

int	is_redirect_out(char *cmd)
{
	return (cmd[0] == '>');
}

int	is_heredoc(char *cmd)
{
	return (cmd && ft_strlen(cmd) >= 2 && cmd[0] == '<' && cmd[1] == '<');
}

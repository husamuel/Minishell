/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 16:25:23 by husamuel          #+#    #+#             */
/*   Updated: 2025/06/03 15:12:41 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

char	*find_command_path(char *cmd, t_env *env)
{
	char	*path_env;
	char	**paths;
	char	*result;

	if (!cmd || access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	path_env = get_env_value(env, "PATH");
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
	{
		free(path_env);
		return (NULL);
	}
	result = search_in_paths(paths, cmd, path_env);
	if (!result)
	{
		ft_free_split(paths);
		free(path_env);
	}
	return (result);
}

int	token_has_redirection(t_token *token)
{
	t_token	*curr;

	curr = token->next;
	while (curr)
	{
		if (curr->type == CMD_REDIRECT_OUT || curr->type == CMD_REDIRECT_IN
			|| curr->type == CMD_APPEND || curr->type == CMD_HEREDOC)
			return (1);
		curr = curr->next;
	}
	return (0);
}

static int	handle_heredoc_alone(t_token *token)
{
	int	fd;

	if (token->type == CMD_HEREDOC && token->next && !token->prev)
	{
		fd = setup_heredoc(token->next->cmd);
		if (fd >= 0)
		{
			close(fd);
			return (0);
		}
		return (1);
	}
	return (-1);
}

static int	handle_builtin_or_expr(t_token *token, t_mini *ms)
{
	if (token_has_redirection(token))
		return (execute_external_builtin(token, ms));
	return (execute_simple_command(token, ms));
}

int	execute_token(t_token *token, t_mini *ms)
{
	int	result;

	if (!token || token->type == CMD_NONE)
		return (0);
	result = handle_heredoc_alone(token);
	if (result != -1)
		return (result);
	if (!is_valid_command(token, ms))
	{
		ft_putstr_fd("minishell: command not found\n", 2);
		return (127);
	}
	if (token->type == CMD_BUILDIN || token->type == CMD_EXPR)
		return (handle_builtin_or_expr(token, ms));
	else if (token->type == CMD_EXEC)
		return (execute_external_command(token, ms));
	return (0);
}

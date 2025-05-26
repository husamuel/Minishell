/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 16:25:23 by husamuel          #+#    #+#             */
/*   Updated: 2025/05/26 09:37:26 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

static char	*build_full_path(char *path, char *cmd)
{
	char	*temp;
	char	*full_path;

	temp = ft_strjoin(path, "/");
	if (!temp)
		return (NULL);
	full_path = ft_strjoin_free(temp, cmd);
	return (full_path);
}

static char	*search_in_paths(char **paths, char *cmd, char *path_env)
{
	char	*full_path;
	int		i;

	i = 0;
	while (paths[i])
	{
		full_path = build_full_path(paths[i], cmd);
		if (!full_path)
		{
			ft_free_split(paths);
			free(path_env);
			return (NULL);
		}
		if (access(full_path, X_OK) == 0)
		{
			ft_free_split(paths);
			free(path_env);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	return (NULL);
}

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

int	execute_token(t_token *token, t_mini *ms)
{
	if (!token || token->type == CMD_NONE)
		return (0);
	if (!is_valid_command(token))
	{
		ft_putstr_fd("minishell: command not found\n", 2);
		return (127);
	}
	if (token->type == CMD_BUILDIN || token->type == CMD_EXPR)
	{
		if (token_has_redirection(token))
			return (execute_external_builtin(token, ms));
		else
			return (execute_simple_command(token, ms));
	}
	else if (token->type == CMD_EXEC)
		return (execute_external_command(token, ms));
	return (0);
}

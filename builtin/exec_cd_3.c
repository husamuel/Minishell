/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cd_3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 18:54:19 by gtretiak          #+#    #+#             */
/*   Updated: 2025/05/22 12:43:01 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

int	change_dir(char *oldpwd, char *pwd, t_token *next);

void	cd_dollar(t_token *next, t_mini *mini)
{
	char	*oldpwd;
	char	*pwd;

	(void)mini;
	if (ft_strcmp(next->cmd, "$0") == 0)
		printf("minishell: cd: minishell: No such file or directory\n");
	else
	{
		oldpwd = getcwd(NULL, 0);
		if (!oldpwd)
		{
			perror("getcwd failed");
			return ;
		}
		pwd = expand_var("HOME", mini->export);
		if (!pwd)
		{
			printf("minishell: cd: HOME not set\n");
			return ;
		}
		if (!change_dir(oldpwd, pwd, next))
			return ;
		update_var(oldpwd, pwd, mini);
		free_pwd(oldpwd, pwd);
	}
}

int	change_dir(char *oldpwd, char *pwd, t_token *next)
{
	if (chdir(pwd) == -1)
	{
		printf("minishell: cd: %s: %s\n", next->cmd, strerror(errno));
		free_pwd(oldpwd, pwd);
		return (0);
	}
	return (1);
}

int	list_size(t_token *token)
{
	int	i;

	i = 0;
	while (token)
	{
		token = token->next;
		i++;
	}
	return (i);
}

void	exec_cd_point(t_token *token, t_mini *mini)
{
	char	*oldpwd;

	(void)token;
	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
	{
		perror("getcwd failed");
		return ;
	}
	update_var(oldpwd, oldpwd, mini);
	free(oldpwd);
}

int	check_fullpath(t_token *next, t_mini *mini)
{
	char	*pwd;
	char	*oldpwd;

	pwd = NULL;
	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
	{
		perror("getcwd failed");
		return (0);
	}
	pwd = ft_strdup(next->cmd);
	if (!pwd)
		return (0);
	if (chdir(pwd) == 0)
	{
		update_var(oldpwd, pwd, mini);
		free_pwd(oldpwd, pwd);
		return (1);
	}
	free_pwd(oldpwd, pwd);
	return (0);
}

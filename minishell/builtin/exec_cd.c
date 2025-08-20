/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 18:54:36 by gtretiak          #+#    #+#             */
/*   Updated: 2025/06/01 13:49:27 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

void	exec_cd_4(t_token *token, t_mini *mini);
void	exec_cd_3(t_token *token, t_mini *mini);
void	exec_cd_2(t_token *token, t_mini *mini);
void	exec_cd_1(t_token *token, t_mini *mini);

void	exec_cd(t_token *token, t_mini *mini)
{
	t_token	*next;

	next = token->next;
	if (list_size(token) > 2)
		printf("minishell: cd: too many arguments\n");
	else if (!token->next || ft_strcmp(token->next->cmd, "~") == 0)
		exec_cd_1(token, mini);
	else if (ft_strcmp(token->next->cmd, "..") == 0)
		exec_cd_2(token, mini);
	else if (ft_strcmp(token->next->cmd, ".") == 0)
		exec_cd_point(token, mini);
	else if (ft_strcmp(token->next->cmd, "-") == 0)
		exec_cd_4(token, mini);
	else if (next->cmd[0] == '$' && ft_isalnum(next->cmd[1]))
		cd_dollar(next, mini);
	else
		exec_cd_3(token, mini);
	setup_signals();
}

void	exec_cd_4(t_token *token, t_mini *mini)
{
	char	*oldpwd;
	char	*pwd;

	(void)token;
	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
	{
		perror("getcwd failed");
		return ;
	}
	pwd = expand_var("OLDPWD", mini->export);
	if (!pwd)
	{
		printf("minishell: cd: OLDPWD not set\n");
		return ;
	}
	if (chdir(pwd) == -1)
	{
		printf("minishell: cd: %s: %s\n", token->next->cmd, strerror(errno));
		free_pwd(oldpwd, pwd);
		return ;
	}
	printf("%s\n", pwd);
	update_var(oldpwd, pwd, mini);
	free_pwd(oldpwd, pwd);
}

void	exec_cd_3(t_token *token, t_mini *mini)
{
	char	*pwd;
	char	*oldpwd;
	char	*temp;

	pwd = NULL;
	temp = NULL;
	if (check_fullpath(token->next, mini))
		return ;
	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
	{
		perror("getcwd failed");
		return ;
	}
	temp = ft_strjoin(oldpwd, "/");
	pwd = ft_strjoin(temp, token->next->cmd);
	free(temp);
	if (chdir(pwd) == -1)
	{
		printf("minishell: cd: %s: %s\n", token->next->cmd, strerror(errno));
		free_pwd(oldpwd, pwd);
		return ;
	}
	update_var(oldpwd, pwd, mini);
	free_pwd(oldpwd, pwd);
}

void	exec_cd_2(t_token *token, t_mini *mini)
{
	char	*pwd;
	char	*oldpwd;

	(void)token;
	pwd = NULL;
	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
	{
		perror("getcwd failed");
		return ;
	}
	pwd = get_new_cwd(oldpwd);
	if (chdir(pwd) == -1)
	{
		printf("minishell: cd: %s: %s\n", pwd, strerror(errno));
		free_pwd(oldpwd, pwd);
		return ;
	}
	update_var(oldpwd, pwd, mini);
	free_pwd(oldpwd, pwd);
}

void	exec_cd_1(t_token *token, t_mini *mini)
{
	char	*oldpwd;
	char	*home;

	if (!token->next && !expand_var("HOME", mini->export))
	{
		printf("minishell: cd: HOME not set\n");
		return ;
	}
	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
	{
		perror("getcwd failed");
		return ;
	}
	home = getenv("HOME");
	if (chdir(home) == -1)
	{
		printf("minishell: cd: %s: %s\n", home, strerror(errno));
		free(oldpwd);
		return ;
	}
	update_var(oldpwd, home, mini);
	free(oldpwd);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cd_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 18:54:04 by gtretiak          #+#    #+#             */
/*   Updated: 2025/04/23 18:54:07 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../minishell.h"

void	update_var(char *oldpwd, char *pwd, t_mini *mini)
{
	t_env	*ev;

	ev = mini->export;
	while (ev)
	{
		if (ft_strcmp(ev->var, "OLDPWD") == 0)
		{
			ev->content = ft_strjoin("OLDPWD=", oldpwd);
		}
		else if (ft_strcmp(ev->var, "PWD") == 0)
		{
			ev->content = ft_strjoin("PWD=", pwd);
		}
		ev = ev->next;
	}
}

// return var content with malloc or NULL if not found
char	*expand_var(char *var, t_env *ev)
{
	int		i;
	char	*content;

	i = ft_strlen(var) + 1;
	content = NULL;
	while (ev)
	{
		if (ft_strcmp(ev->var, var) == 0)
		{
			if (ev->content)
				content = ft_strdup(ev->content + i);
			break ;
		}
		ev = ev->next;
	}
	return (content);
}

void	free_pwd(char *oldpwd, char *pwd)
{
	if (oldpwd)
		free(oldpwd);
	if (pwd)
		free(pwd);
}

char	*get_new_cwd(char *buffer)
{
	int		i;
	char	*new;

	new = ft_strdup(buffer);
	i = ft_strlen(new) - 1;
	while (i >= 0)
	{
		if (new[i] == '/')
		{
			if (i == 0)
				new[++i] = '\0';
			else
				new[i] = '\0';
			break ;
		}
		i--;
	}
	return (new);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	if (!s1 || !s2)
	{
		printf("Error strcmp: NULL string");
		return (1);
	}
	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

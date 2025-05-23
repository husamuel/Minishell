/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_echo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 18:54:49 by gtretiak          #+#    #+#             */
/*   Updated: 2025/05/22 12:43:13 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

char	*print_echo(char *input, int *i, char *var, t_mini *mini);
void	echo_others(t_token *next, int i, t_mini *mini, char *input);
void	echo_dollar(int *i, char *input, t_mini *mini);
int		check_nl_echo(char *s);

int	has_unclosed_quotes(const char *input)
{
	int		i = 0;
	char	quote = '\0';

	while (input[i])
	{
		if ((input[i] == '\'' || input[i] == '\"'))
		{
			if (!quote)
				quote = input[i];
			else if (quote == input[i])
				quote = '\0';
		}
		i++;
	}
	return (quote != '\0');
}

void	exec_echo(t_token *token, t_mini *mini)
{
	if (has_unclosed_quotes(mini->input))
	{
		mini->exit_status = 2;
		return ;
	}
	if (ft_strchr(mini->input, '\'') || ft_strchr(mini->input, '\"') || ft_strchr(mini->input, '$'))
	{
		echo_others(token->next, 0, mini, mini->input + 5);
		return ;
	}

	int		i = 1;
	int		newline = 1;

	if (token->args[1] && ft_strncmp(token->args[1], "-n", 3) == 0)
	{
		newline = 0;
		i++;
	}

	while (token->args[i])
	{
		char *arg = token->args[i];

		if (arg[0] == '$')
		{
			if (arg[1] == '?')
				printf("%d", mini->exit_status);
			else
			{
				char *value = get_env_value(mini->export, arg + 1);
				if (value)
				{
					printf("%s", value);
					free(value);
				}
			}
		}
		else
			printf("%s", arg);

		if (token->args[i + 1])
			printf(" ");
		i++;
	}

	if (newline)
		printf("\n");
}


void	echo_dollar(int *i, char *input, t_mini *mini)
{
	char	*var;

	var = NULL;
	if (input[*i + 1] == '?')
	{
		printf("%s", mini->token->args[1]);
		*i += 2;
		return ;
	}
	else if (ft_isdigit(input[*i + 1]))
	{
		if (input[*i + 1] == '0')
			printf("minishell");
		*i += 2;
	}
	else if (ft_isalpha(input[*i + 1]))
	{
		var = ft_strdup(&input[++(*i)]);
		print_echo(input, i, var, mini);
	}
}

char	*print_echo(char *input, int *i, char *var, t_mini *mini)
{
	char	*s;
	int		j;

	j = 0;
	while (ft_isalnum(var[j]) || var[j] == '_')
		j++;
	var[j] = '\0';
	s = expand_var(var, mini->export);
	free(var);
	if (s)
	{
		printf("%s", s);
		free(s);
	}
	while (ft_isalnum(input[*i]) || input[*i] == '_')
		(*i)++;
	return (s);
}

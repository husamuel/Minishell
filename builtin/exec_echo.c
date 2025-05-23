/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_echo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 18:54:49 by gtretiak          #+#    #+#             */
/*   Updated: 2025/05/23 18:01:07 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

char	*print_echo(char *input, int *i, char *var, t_mini *mini);
void	echo_others(t_token *next, int i, t_mini *mini, char *input);
void	echo_dollar(int *i, char *input, t_mini *mini);
int		check_nl_echo(char *s);

int	has_unclosed_quotes(const char *input)
{
	int		i;
	char	quote;

	i = 0;
	quote = '\0';
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

static void	handle_dollar_variable(t_token *token, t_mini *mini, int i)
{
	char	*arg;
	char	*value;

	arg = token->args[i];
	if (arg[1] == '?')
		printf("%d", mini->exit_status);
	else
	{
		value = get_env_value(mini->export, arg + 1);
		if (value)
		{
			printf("%s", value);
			free(value);
		}
	}
}

static void	print_args(t_token *token, t_mini *mini, int start_index)
{
	int		i;
	char	*arg;

	i = start_index;
	while (token->args[i])
	{
		arg = token->args[i];
		if (arg[0] == '$')
			handle_dollar_variable(token, mini, i);
		else
			printf("%s", arg);
		if (token->args[i + 1])
			printf(" ");
		i++;
	}
}

static int	check_newline_flag(t_token *token)
{
	if (token->args[1] && ft_strncmp(token->args[1], "-n", 3) == 0)
		return (0);
	return (1);
}

void	exec_echo(t_token *token, t_mini *mini)
{
	int	start_index;
	int	newline;

	if (has_unclosed_quotes(mini->input))
	{
		mini->exit_status = 2;
		return ;
	}
	if (ft_strchr(mini->input, '\'') || ft_strchr(mini->input, '\"')
		|| ft_strchr(mini->input, '$'))
	{
		echo_others(token->next, 0, mini, mini->input + 5);
		return ;
	}
	start_index = 1;
	newline = check_newline_flag(token);
	if (!newline)
		start_index++;
	print_args(token, mini, start_index);
	if (newline)
		printf("\n");
}

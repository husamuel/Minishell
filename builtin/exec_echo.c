/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_echo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 18:54:49 by gtretiak          #+#    #+#             */
/*   Updated: 2025/06/04 13:13:20 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

char	*print_echo(char *input, int *i, char *var, t_mini *mini);
void	echo_others(t_token *next, int i, t_mini *mini, char *input);
void	echo_dollar(int *i, char *input, t_mini *mini);
int		check_nl_echo(char *s);

static void	handle_dollar_variable(t_token *token, t_mini *mini, int i)
{
	char	*arg;
	char	*value;

	arg = token->args[i];
	if (arg[1] == '?')
		printf("%d", g_exit_status);
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

static void	ft_handle_home(void)
{
	char	*home;

	home = getenv("HOME");
	if (home)
		printf("%s", home);
	else
		printf("~");
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
		else if (!ft_strcmp(arg, "~"))
			ft_handle_home();
		else
			printf("%s", arg);
		if (token->args[i + 1])
			printf(" ");
		i++;
	}
}

static int	count_newline_flags(t_token *token, int *newline)
{
	int	count;
	int	i;

	count = 0;
	*newline = 1;
	while (token)
	{
		if (token->cmd[0] == '-')
		{
			i = 1;
			while (token->cmd[i] == 'n')
				i++;
			if (token->cmd[i] == '\0')
			{
				count++;
				*newline = 0;
				token = token->next;
				continue ;
			}
		}
		break ;
	}
	return (count);
}

void	exec_echo(t_token *token, t_mini *mini)
{
	int	start_index;
	int	newline;

	if (has_unclosed_quotes(mini->input))
	{
		g_exit_status = 2;
		return ;
	}
	start_index = 1 + count_newline_flags(token->next, &newline);
	if (ft_strchr(mini->input, '\'') || ft_strchr(mini->input, '\"')
		|| ft_strchr(mini->input, '$'))
	{
		echo_others(token->next, newline, mini, mini->input + 5);
		g_exit_status = 0;
		return ;
	}
	print_args(token, mini, start_index);
	if (newline)
		printf("\n");
	g_exit_status = 0;
	setup_signals();
}

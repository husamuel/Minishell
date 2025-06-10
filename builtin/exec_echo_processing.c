/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_echo_processing.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 18:37:12 by gtretiak          #+#    #+#             */
/*   Updated: 2025/06/10 22:41:01 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

char	*print_echo(char *input, int *i, char *var, t_mini *mini);
void	echo_others(t_token *next, int i, t_mini *mini, char *input);
void	echo_dollar(int *i, char *input, t_mini *mini);

static int	ft_handle_quote_start(char *input, int *i,
		int *q_flag, char *q_type)
{
	if (!*q_flag)
	{
		*q_flag = 1;
		*q_type = input[*i];
		if (input[*i + 1] == *q_type)
		{
			(*i) += 2;
			*q_flag = 0;
			*q_type = '\0';
			return (1);
		}
		(*i)++;
		return (1);
	}
	return (0);
}

static int	ft_handle_quote_end(char *input, int *i, int *q_flag, char *q_type)
{
	if (input[*i] == *q_type)
	{
		*q_flag = 0;
		*q_type = '\0';
		(*i)++;
		return (1);
	}
	return (0);
}

int	ft_process_quotes(char *input, int *i, int *q_flag, char *q_type)
{
	if ((input[*i] == '\"' || input[*i] == '\'')
		&& (*i == 0 || input[*i - 1] != '\\'))
	{
		if (ft_handle_quote_start(input, i, q_flag, q_type))
			return (1);
		if (*q_flag && ft_handle_quote_end(input, i, q_flag, q_type))
			return (1);
	}
	return (0);
}

void	echo_others(t_token *next, int i, t_mini *mini, char *input)
{
	char	quote_type;
	int		nl_flag;
	int		in_quotes;

	ft_handle_flag(&next, &nl_flag);
	in_quotes = 0;
	quote_type = '\0';
	i = ft_skip_n_flags(input, &nl_flag);
	while (input && input[i])
	{
		ft_process_quotes(input, &i, &in_quotes, &quote_type);
		if (!input[i])
			break ;
		if ((in_quotes && quote_type == '\"' && input[i] == '$')
			|| (!in_quotes && input[i] == '$'))
			echo_dollar(&i, input, mini);
		else
		{
			printf("%c", input[i]);
			i++;
		}
	}
	if (!nl_flag)
		printf("\n");
}

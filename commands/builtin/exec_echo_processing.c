/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_echo_processing.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 18:37:12 by gtretiak          #+#    #+#             */
/*   Updated: 2025/05/07 16:19:33 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../minishell.h"

char	*print_echo(char *input, int *i, char *var, t_mini *mini);
void	echo_others(t_token *next, int i, t_mini *mini, char *input);
void	echo_dollar(int *i, char *input, t_mini *mini);

static void	ft_process_quotes(char *input, int *i, int *q_flag, char *q_type)
{
	if ((input[*i] == '\"' || input[*i] == '\'')
		&& (*i == 0 || input[*i - 1] != '\\'))
	{
		if (!*q_flag)
		{
			*q_flag = 1;
			*q_type = input[*i];
			(*i)++;
		}
		else if (input[*i] == *q_type)
		{
			*q_flag = 0;
			*q_type = '\0';
			(*i)++;
		}
		else
		{
			printf("%c", input[*i]);
			(*i)++;
		}
	}
}

/*
static void	ft_adjust_starting_pos(t_token *next, char **input)
{
	char	*next_pos;

	if (!next)
		return ;
	while (**input == ' ')
		(*input)++;
	next_pos = ft_custom_strstr(*input, next->cmd);
	if (next_pos)
		*input = next_pos;
}
	+ call from echo_others:
	ft_adjust_starting_pos(next, &input);
*/

void	echo_others(t_token *next, int i, t_mini *mini, char *input)
{
	int		nl_flag;
	int		in_quotes;
	char	quote_type;

	ft_handle_flag(&next, &nl_flag);
	in_quotes = 0;
	quote_type = '\0';
	i = ft_skip_n_flags(input, &nl_flag);
	while (input && input[i])
	{
		ft_process_quotes(input, &i, &in_quotes, &quote_type);
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

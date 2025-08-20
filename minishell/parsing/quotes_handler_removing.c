/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_handler_removing.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 16:07:29 by gtretiak          #+#    #+#             */
/*   Updated: 2025/04/27 16:07:33 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

static void	ft_check_for_literals(char *str, t_token *token, int end)
{
	int		i;

	i = 0;
	while (++i < end)
	{
		if (str[i] == '|' || str[i] == '<'
			|| str[i] == '>' || str[i] == ';')
		{
			token->is_literal = 1;
			break ;
		}
	}
}

static char	*ft_unquote_it(char *str, int len)
{
	char	*result;
	int		i;

	i = 0;
	result = malloc(len);
	if (!result)
		return (NULL);
	i = 0;
	while (++i < len)
		result[i - 1] = str[i];
	result[len - 1] = '\0';
	return (result);
}

char	*remove_quotes(char *str, t_token *token)
{
	int		len;
	int		end;
	char	*result;

	len = ft_strlen(str);
	end = len - 1;
	if (len < 2)
		return (str);
	if ((str[0] == '\'' && str[end] == '\'')
		|| (str[0] == '\"' && str[end] == '\"'))
	{
		token->quoted = 1;
		if (str[0] == '\'')
			token->quoted_type = CMD_SINGLE_QUOTE;
		else
			token->quoted_type = CMD_DOUBLE_QUOTE;
		ft_check_for_literals(str, token, end);
		result = ft_unquote_it(str, len);
		if (result)
		{
			free(str);
			return (result);
		}
	}
	return (str);
}

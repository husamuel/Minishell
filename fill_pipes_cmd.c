/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_pipes_cmd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 10:23:26 by husamuel          #+#    #+#             */
/*   Updated: 2025/05/26 13:12:13 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strjoin_space(const char *s1, const char *s2)
{
	size_t	len1;
	size_t	len2;
	char	*result;
	size_t	i;
	size_t	j;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	result = malloc(len1 + len2 + 2);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (s1 && s1[i])
	{
		result[i] = s1[i];
		i++;
	}
	if (len1 > 0)
		result[i++] = ' ';
	while (s2 && s2[j])
		result[i++] = s2[j++];
	result[i] = '\0';
	return (result);
}

int	count_pipe_segments(t_token *token)
{
	int	count;

	count = 1;
	while (token)
	{
		if (token->type == CMD_PIPE)
			count++;
		token = token->next;
	}
	return (count);
}

char	*join_command_segment(t_token **token)
{
	char	*segment;
	char	*tmp;

	segment = NULL;
	while (*token && (*token)->type != CMD_PIPE)
	{
		tmp = segment;
		if (!segment)
			segment = ft_strdup((*token)->cmd);
		else
			segment = ft_strjoin_space(segment, (*token)->cmd);
		if (!segment)
		{
			free(tmp);
			return (NULL);
		}
		free(tmp);
		*token = (*token)->next;
	}
	if (*token && (*token)->type == CMD_PIPE)
		*token = (*token)->next;
	return (segment);
}

int	*extract_command_types(t_token *token, int count)
{
	int		*types;
	int		i;
	t_token	*start;

	types = malloc(sizeof(int) * count);
	if (!types)
		return (NULL);
	i = 0;
	while (token && i < count)
	{
		start = token;
		while (token && token->type != CMD_PIPE)
			token = token->next;
		types[i++] = start->type;
		if (token && token->type == CMD_PIPE)
			token = token->next;
	}
	return (types);
}

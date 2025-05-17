/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 12:26:21 by gtretiak          #+#    #+#             */
/*   Updated: 2025/05/02 17:13:51 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void free_args_array(char **args)
{
    int i;
    
    if (!args)
        return;
    
    i = 0;
    while (args[i])
    {
        free(args[i]);
        args[i] = NULL; // Set to NULL after freeing
        i++;
    }
    free(args);
}

void ft_free_minishell(t_mini *ms, int code)
{
    (void)code; // Ignore code for now, free all tokens
    if (!ms)
        return;

    // Free the entire token list
    if (ms->token)
    {
        free_tokens(ms->token);
        ms->token = NULL;
    }

    // Free other minishell resources if needed
    if (ms->input)
        free(ms->input);
    if (ms->prompt)
        free(ms->prompt);
    if (ms->export)
        free_env_list(ms->export);
    if (ms->state)
    {
        free(ms->state);
        ms->state = NULL;
    }
}

void	free_env_list(t_env *head)
{
	t_env	*current;
	t_env	*next;

	current = head;
	while (current)
	{
		next = current->next;
		if (current->content)
			free(current->content);
		if (current->var)
			free(current->var);
		free(current);
		current = next;
	}
}

void free_token_content(t_token *token)
{
    if (token->cmd)
    {
        // Only free cmd if it's not referenced by args or args_file
        if (token->args && token->args[0] != token->cmd &&
            token->args_file && token->args_file[0] != token->cmd)
        {
            free(token->cmd);
            token->cmd = NULL;
        }
    }
    if (token->args_file)
        free_args_array(token->args_file);
    if (token->args)
        free_args_array(token->args);
}

void	free_tokens(t_token *token)
{
	t_token	*tmp;

	while (token)
	{
		tmp = token->next;
		free_token_content(token);
		free(token);
		token = tmp;
	}
}

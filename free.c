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

void ft_free_minishell(t_mini *ms, int code)
{
    t_token *current;
    t_token *next;
    
    if (!ms || !ms->state)
        return;
    
    current = ms->state->curr;
    
    while (current)
    {
        next = current->next;
        
        if (current->args)
        {
            int i = 0;
            while (current->args[i])
            {
                free(current->args[i]);
                current->args[i] = NULL;
                i++;
            }
            free(current->args);
            current->args = NULL;
        }
        
        if (current->args_file)
        {
            int i = 0;
            while (current->args_file[i])
            {
                free(current->args_file[i]);
                current->args_file[i] = NULL;
                i++;
            }
            free(current->args_file);
            current->args_file = NULL;
        }
        if (code == 100)
            current = next;
        else
            break;
    }
}

void free_env_list(t_env *head)
{
    t_env *current;
    t_env *next;
    
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

void free_tokens(t_token *token)
{
    t_token *tmp;

    while (token)
    {
        tmp = token->next;
        if (token->cmd)
        {
            free(token->cmd);
            token->cmd = NULL;
        }
        if (token->args_file)
        {
            int i = 0;
            while (token->args_file[i])
            {
                free(token->args_file[i]);
                token->args_file[i] = NULL;
                i++;
            }
            free(token->args_file);
            token->args_file = NULL;
        }
        if (token->args)
        {
            int i = 0;
            while (token->args[i])
            {
                free(token->args[i]);
                token->args[i] = NULL;
                i++;
            }
            free(token->args);
            token->args = NULL;
        }
        
        free(token);
        token = tmp;
    }
}
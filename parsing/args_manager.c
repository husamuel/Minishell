/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_manager.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 17:39:54 by gtretiak          #+#    #+#             */
/*   Updated: 2025/04/27 17:41:18 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

void add_to_args(t_token *token, char *arg)
{
    int i;
    char **new_args;
    
    i = 0;
    if (token->args)
    {
        while (token->args[i])
            i++;
    }
    
    new_args = malloc(sizeof(char *) * (i + 2));
    if (!new_args)
        return;
    
    if (token->args)
    {
        i = 0;
        while (token->args[i])
        {
            new_args[i] = token->args[i];
            i++;
        }
        free(token->args);
    }
    
    new_args[i] = ft_strdup(arg);
    if (!new_args[i])
    {
        free(new_args);
        return;
    }
    
    new_args[i + 1] = NULL;
    token->args = new_args;
}

void add_to_args_file(t_token *token, char *arg)
{
    int i;
    char **new_args;
    
    i = 0;
    if (token->args_file)
    {
        while (token->args_file[i])
            i++;
    }
    
    new_args = malloc(sizeof(char *) * (i + 2));
    if (!new_args)
        return;
    
    if (token->args_file)
    {
        i = 0;
        while (token->args_file[i])
        {
            new_args[i] = token->args_file[i];
            i++;
        }
        free(token->args_file);  // Libera apenas o array, não os strings
    }
    
    new_args[i] = ft_strdup(arg);
    if (!new_args[i])  // Verificação de erro para ft_strdup
    {
        free(new_args);
        return;
    }
    
    new_args[i + 1] = NULL;
    token->args_file = new_args;
}
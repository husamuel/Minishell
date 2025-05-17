/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 12:26:21 by gtretiak          #+#    #+#             */
/*   Updated: 2025/05/17 16:15:00 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Libera um array de strings e define seus elementos como NULL
 */
void	free_args_array(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		free(args[i]);
		args[i] = NULL; // Set to NULL after freeing
		i++;
	}
	free(args);
}

/**
 * Libera recursos associados ao minishell
 */
void	ft_free_minishell(t_mini *ms, int code)
{
	if (!ms)
		return ;
	
	// Free the entire token list
	if (ms->token)
	{
		free_tokens(ms->token);
		ms->token = NULL;
	}
	
	// Free other minishell resources if needed
	if (ms->input)
	{
		free(ms->input);
		ms->input = NULL;
	}
	
	if (ms->prompt)
	{
		free(ms->prompt);
		ms->prompt = NULL;
	}
	
	if (code != -1)
	{
		// Libera recursos globais apenas se for saída final
		if (ms->export)
		{
			free_env_list(ms->export);
			ms->export = NULL;
		}
		
		// Não liberar state se for alocado na stack (que parece ser o caso)
		// Se mudar isso, verifique se state é alocado com malloc em algum lugar
	}
}

/**
 * Libera uma lista de variáveis de ambiente
 */
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

/**
 * VERSÃO CORRIGIDA: Libera conteúdo de um token com cuidado para evitar double free
 */
void	free_token_content(t_token *token)
{
	if (!token)
		return ;
	
	// Primeiro, verificar e marcar todos os ponteiros compartilhados
	// para evitar double free
	int has_shared_cmd = 0;
	
	// Verificar se cmd é usado em args ou args_file
	if (token->cmd && token->args)
	{
		int i = 0;
		while (token->args[i])
		{
			if (token->args[i] == token->cmd)
			{
				has_shared_cmd = 1;
				break;
			}
			i++;
		}
	}
	
	if (!has_shared_cmd && token->cmd && token->args_file)
	{
		int i = 0;
		while (token->args_file[i])
		{
			if (token->args_file[i] == token->cmd)
			{
				has_shared_cmd = 1;
				break;
			}
			i++;
		}
	}
	
	// Liberar args_file (mas não seus conteúdos que possam ser compartilhados)
	if (token->args_file)
	{
		int i = 0;
		while (token->args_file[i])
		{
			// Liberar apenas se não for ponteiro compartilhado com cmd
			if (token->cmd && token->args_file[i] == token->cmd)
				; // Não liberar, será liberado depois
			else
				free(token->args_file[i]);
			i++;
		}
		free(token->args_file);
		token->args_file = NULL;
	}
	
	// Liberar args (mas não seus conteúdos que possam ser compartilhados)
	if (token->args)
	{
		int i = 0;
		while (token->args[i])
		{
			// Liberar apenas se não for ponteiro compartilhado com cmd
			if (token->cmd && token->args[i] == token->cmd)
				; // Não liberar, será liberado depois
			else
				free(token->args[i]);
			i++;
		}
		free(token->args);
		token->args = NULL;
	}
	
	// Agora é seguro liberar cmd
	if (token->cmd)
	{
		free(token->cmd);
		token->cmd = NULL;
	}
}

/**
 * Libera uma lista de tokens
 */
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
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 19:02:26 by gtretiak          #+#    #+#             */
/*   Updated: 2024/11/08 14:40:29 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char *ft_strdup(char *src)
{
    int i = 0;
    char *res;
    
    if (!src)
        return (NULL);
    
    // Count the length of the source string
    while(src[i])
        i++;
    
    // Allocate memory for the new string including null terminator
    res = (char*)malloc(sizeof(char) * (i + 1)); // +1 for null terminator
    if (res == NULL)
        return (NULL);
    
    // Copy the string
    i = 0;
    while(src[i])
    {
        res[i] = src[i];
        i++;
    }
    
    // Add null terminator
    res[i] = '\0';
    
    return (res);
}